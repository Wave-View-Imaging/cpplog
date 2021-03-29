#include <string>
#include <fstream>
#include <thread>
#include <chrono>

#include <cpplog/log.hpp>

int main() {
    std::ofstream error_log_file ("error_log.txt", std::ofstream::out);
    logging::add_message_sink(error_log_file, logging::Severity::Error);
    logging::add_message_sink(std::cout, 
            logging::Severity::Trace | logging::Severity::Debug | logging::Severity::Warn | logging::Severity::Error | logging::Severity::Info | logging::Severity::Fatal);

    TRACE << "trace log" << std::endl;
    DEBUG << "debug log" << std::endl;
    INFO << "info log" << std::endl;
    WARN << "warn log" << std::endl;
    ERROR << "error log" << std::endl;
    FATAL << "fatal log" << std::endl;

    std::thread t1{[]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        ERROR << "error in thread" << 1 << std::endl;
    }};
    std::thread t2{[]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        WARN << "warning in thread" << 2 << std::endl;
    }};
    t2.join();
    t1.join();

    return EXIT_SUCCESS;
}
