#include <catch2/catch.hpp>
#include <cpplog/log.hpp>
#include <cpplog/log_parser.hpp>

SCENARIO("Messages send to a given log print correctly to the message sink", "[Severity]") {
    GIVEN("that an ostream is added as a sink") {
        std::ostringstream ss{};
        logging::add_message_sink(ss,
            logging::Severity::Trace | logging::Severity::Debug | logging::Severity::Warn | logging::Severity::Error | logging::Severity::Info | logging::Severity::Fatal);

        WHEN("messages streamed to TRACE are send to the correct log") {
            std::string log_msg = "message to be logged";
            TRACE << log_msg << std::endl;

            THEN("the message is send to the the stream") {
                auto log_record = logging::parse_log_message_to_record(ss.str());
                REQUIRE(log_record.severity == logging::Severity::Trace);
                REQUIRE(log_record.message == log_msg);
            }
        }

        WHEN("messages streamed to DEBUG are send to the correct log") {
            std::string log_msg = "message to be logged";
            DEBUG << log_msg << std::endl;

            THEN("the message is send to the the stream") {
                auto log_record = logging::parse_log_message_to_record(ss.str());
                REQUIRE(log_record.severity == logging::Severity::Debug);
                REQUIRE(log_record.message == log_msg);
            }
        }

        WHEN("messages streamed to INFO are send to the correct log") {
            std::string log_msg = "message to be logged";
            INFO << log_msg << std::endl;

            THEN("the message is send to the the stream") {
                auto log_record = logging::parse_log_message_to_record(ss.str());
                REQUIRE(log_record.severity == logging::Severity::Info);
                REQUIRE(log_record.message == log_msg);
            }
        }

        WHEN("messages streamed to WARN are send to the correct log") {
            std::string log_msg = "message to be logged";
            WARN << log_msg << std::endl;

            THEN("the message is send to the the stream") {
                auto log_record = logging::parse_log_message_to_record(ss.str());
                REQUIRE(log_record.severity == logging::Severity::Warn);
                REQUIRE(log_record.message == log_msg);
            }
        }
        
        WHEN("messages streamed to ERROR are send to the correct log") {
            std::string log_msg = "message to be logged";
            ERROR << log_msg << std::endl;

            THEN("the message is send to the the stream") {
                auto log_record = logging::parse_log_message_to_record(ss.str());
                REQUIRE(log_record.severity == logging::Severity::Error);
                REQUIRE(log_record.message == log_msg);
            }
        }

        WHEN("messages streamed to FATAL are send to the correct log") {
            std::string log_msg = "message to be logged";
            FATAL << log_msg << std::endl;

            THEN("the message is send to the the stream") {
                auto log_record = logging::parse_log_message_to_record(ss.str());
                REQUIRE(log_record.severity == logging::Severity::Fatal);
                REQUIRE(log_record.message == log_msg);
            }
        }
    }
}
