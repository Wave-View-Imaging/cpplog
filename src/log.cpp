#include <vector>
#include <iostream>
#include <string>

#include <cpplog/log.hpp>

namespace {
    std::vector<std::shared_ptr<logging::Sync_Ostream>> msg_sinks;
    std::vector<logging::Observer_Function> msg_observers;
}

namespace logging {

    Severity operator&(Severity lhs, Severity rhs) {
        return static_cast<Severity> (
                static_cast<std::underlying_type<Severity>::type>(lhs) &
                static_cast<std::underlying_type<Severity>::type>(rhs)
                );
    }

    Severity operator|(Severity lhs, Severity rhs) {
        return static_cast<Severity> (
                static_cast<std::underlying_type<Severity>::type>(lhs) |
                static_cast<std::underlying_type<Severity>::type>(rhs)
                );
    }

    std::ostream& operator<<(std::ostream& out, const Severity& severity) {
        switch (severity) {
            case Severity::Trace:
                out << "TRACE"; break;
            case Severity::Debug:
                out << "DEBUG"; break;
            case Severity::Info:
                out << "INFO"; break;
            case Severity::Warn:
                out << "WARN"; break;
            case Severity::Error:
                out << "ERROR"; break;
            case Severity::Fatal:
                out << "FATAL"; break;
        }
        return out;
    }

    Log_Sink_ID add_message_sink(std::ostream& sink, const Severity& severity) {
        auto msg_sink = std::make_shared<Sync_Ostream>(sink);

        if ((severity & Severity::Trace) == Severity::Trace) {
            trace_log.add_message_sink(msg_sink);
        }
        if ((severity & Severity::Debug) == Severity::Debug) {
            debug_log.add_message_sink(msg_sink);
        }
        if ((severity & Severity::Info) == Severity::Info) {
            info_log.add_message_sink(msg_sink);
        }
        if ((severity & Severity::Warn) == Severity::Warn) {
            warn_log.add_message_sink(msg_sink);
        }
        if ((severity & Severity::Error) == Severity::Error) {
            error_log.add_message_sink(msg_sink);
        }
        if ((severity & Severity::Fatal) == Severity::Fatal) {
            fatal_log.add_message_sink(msg_sink);
        }

        msg_sinks.push_back(std::move(msg_sink));
        return msg_sinks.size() - 1;
    }

    void remove_message_sink(Log_Sink_ID sink_id) {
        if (sink_id < 0 && sink_id >= msg_sinks.size()) return;

        msg_sinks[sink_id] = nullptr;
    }

    Log_Observer_ID add_observer(
            Observer_Function func,
            const Severity& severity
            ) {
        msg_observers.push_back(std::move(func));

        if ((severity & Severity::Trace) == Severity::Trace) {
            trace_log.add_observer(std::ref(msg_observers.back()));
        }
        if ((severity & Severity::Debug) == Severity::Debug) {
            debug_log.add_observer(std::ref(msg_observers.back()));
        }
        if ((severity & Severity::Info) == Severity::Info) {
            info_log.add_observer(std::ref(msg_observers.back()));
        }
        if ((severity & Severity::Warn) == Severity::Warn) {
            warn_log.add_observer(std::ref(msg_observers.back()));
        }
        if ((severity & Severity::Error) == Severity::Error) {
            error_log.add_observer(std::ref(msg_observers.back()));
        }
        if ((severity & Severity::Fatal) == Severity::Fatal) {
            fatal_log.add_observer(std::ref(msg_observers.back()));
        }

        return msg_observers.size() - 1;
    }

    void remove_observer(Log_Observer_ID observer_id) {
        if (observer_id < 0 && observer_id >= msg_observers.size()) return;

        msg_observers[observer_id] = nullptr;
    }
}

logging::Logger trace_log{logging::Severity::Trace};
logging::Logger debug_log{logging::Severity::Debug};
logging::Logger info_log{logging::Severity::Info};
logging::Logger warn_log{logging::Severity::Warn};
logging::Logger error_log{logging::Severity::Error};
logging::Logger fatal_log{logging::Severity::Fatal};
