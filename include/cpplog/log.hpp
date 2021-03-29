#ifndef LOG_H
#define LOG_H

#include <filesystem>
#include <iostream>
#include <vector>
#include <functional>
#include <sstream>

namespace logging {
    enum class Severity {
        Trace = 1,
        Debug = 2,
        Info  = 4,
        Warn  = 8,
        Error = 16,
        Fatal = 32
    };

    Severity operator&(Severity lhs, Severity rhs);
    Severity operator|(Severity lhs, Severity rhs);
    std::ostream& operator<<(std::ostream& out, const Severity& severity);

    class Sync_Ostream : public std::ostream {
        public:
            explicit Sync_Ostream(std::ostream& out = std::cout) : 
                std::ostream(&buffer),
                buffer(out.rdbuf()) {}

            virtual ~Sync_Ostream() = default;

        private:
            class Sync_Ostream_Buffer : public std::streambuf {
                public:
                    explicit Sync_Ostream_Buffer(std::streambuf* other_buf) : buf(other_buf) {}
                    virtual ~Sync_Ostream_Buffer() = default;

                protected:
                    virtual int overflow(int c) {
                        if (c != traits_type::eof()) {
                            std::lock_guard<std::mutex> buf_guard(buf_write_mutex);
                            if (buf != nullptr) buf->sputc(static_cast<char>(c));
                        }
                        return c;
                    }

                    virtual int sync() {
                        std::lock_guard<std::mutex> buf_guard(buf_write_mutex);
                        if (buf != nullptr) buf->pubsync();

                        return 0;
                    }

                private:
                    std::streambuf* buf;
                    std::mutex buf_write_mutex;
            } buffer;
    };

    using Observer_Function = std::function<void(const Severity&, const std::string&)>;
    class Logger : public std::ostream {
        public:
            explicit Logger(Severity severity) :
                std::ostream(&buffer), 
                buffer(*this),
                severity{severity} {}

            virtual ~Logger() = default;

            void add_message_sink(std::shared_ptr<Sync_Ostream> log_sink) {
                msg_sinks.push_back(log_sink);
            }

            void add_observer(std::reference_wrapper<Observer_Function> func) {
                observers.push_back(func);
            }

            Logger& print_prefix(
                    const char* filename_with_path,
                    const char* function_name,
                    const int line_number) {

                const auto remove_quotes = [](std::string&& s) {
                    auto str {std::move(s)};
                    if (str.front() == '"')
                        str.erase(str.begin(), str.begin() + 1);
                    if (str.back() == '"')
                        str.erase(str.end() - 1, str.end());

                    return str;
                };

                const auto now = std::chrono::system_clock::now();
                const auto now_ctime = std::chrono::system_clock::to_time_t(now);
                auto filename_with_quotes = 
                    std::filesystem::path(filename_with_path).filename().string();

                const auto filename = remove_quotes(std::move(filename_with_quotes));

                *this << std::put_time(std::localtime(&now_ctime), "%F %T ");
                *this << filename << ":" << function_name << ":" << line_number;
                *this << " " << severity << ": ";

                return *this;
            }

        private:
            class Log_Buffer : public std::stringbuf {
                public:
                    explicit Log_Buffer(Logger &log) : log(log) {}
                    virtual ~Log_Buffer() = default;

                protected:
                    virtual int overflow(int c) {
                        if (c != traits_type::eof()) {
                            std::lock_guard<std::mutex> buf_guard(buf_write_mutex);
                            buf << static_cast<char>(c);
                        }
                        return c;
                    }

                    virtual int sync() {
                        for (auto& msg_sink : log.msg_sinks) {
                            std::shared_ptr<Sync_Ostream> valid_msg_sink{msg_sink};
                            *valid_msg_sink << buf.str();
                            valid_msg_sink->flush();
                        }

                        for (auto& func : log.observers) {
                            func(log.severity, buf.str());
                        }

                        buf.str("");

                        return 0;
                    }

                private:
                    Logger &log;
                    std::stringstream buf;
                    std::mutex sink_write_mutex;
                    std::mutex buf_write_mutex;
            } buffer;

            Severity severity;
            std::vector<std::weak_ptr<Sync_Ostream>> msg_sinks;
            std::vector<std::reference_wrapper<Observer_Function>> observers;
    };

    using Log_Sink_ID = unsigned int;
    Log_Sink_ID add_message_sink(std::ostream& sink, const Severity& severity);

    void remove_message_sink(Log_Sink_ID sink_id);

    using Log_Observer_ID = unsigned int;
    Log_Observer_ID add_observer(
        std::function<void(const Severity&, const std::string&)> func,
        const Severity& severity
    );

    void remove_observer(Log_Observer_ID observer_id);
}

extern logging::Logger trace_log;
extern logging::Logger debug_log;
extern logging::Logger info_log;
extern logging::Logger warn_log;
extern logging::Logger error_log;
extern logging::Logger fatal_log;

#define TRACE trace_log.print_prefix(__FILE__, __FUNCTION__, __LINE__)
#define DEBUG debug_log.print_prefix(__FILE__, __FUNCTION__, __LINE__)
#define INFO info_log.print_prefix(__FILE__, __FUNCTION__, __LINE__)
#define WARN warn_log.print_prefix(__FILE__, __FUNCTION__, __LINE__)
#define ERROR error_log.print_prefix(__FILE__, __FUNCTION__, __LINE__)
#define FATAL fatal_log.print_prefix(__FILE__, __FUNCTION__, __LINE__)

#endif
