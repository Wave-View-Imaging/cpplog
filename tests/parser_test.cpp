#include <catch2/catch.hpp>
#include <cpplog/log_parser.hpp>

SCENARIO("Log messages can be parsed to retrieve relevant information", "[Parse]") {
    GIVEN("a trace log message") {
        const std::string msg = "2021-03-21 23:33:09 filename.cpp:func_name:7 TRACE: trace log message";
        WHEN("the message is parsed") {
           auto log_record = logging::parse_log_message_to_record(msg); 

            THEN("the parsed date is correct") {
                REQUIRE(log_record.date.year == 2021);
                REQUIRE(log_record.date.month == 3);
                REQUIRE(log_record.date.day == 21);
            }

            THEN("the parsed time is correct") {
                REQUIRE(log_record.time.hour == 23);
                REQUIRE(log_record.time.minute == 33);
                REQUIRE(log_record.time.second == 9);
            }

            THEN("the parsed file information is correct") {
                REQUIRE(log_record.file_info.filename == "filename.cpp");
                REQUIRE(log_record.file_info.function_name == "func_name");
                REQUIRE(log_record.file_info.line_number == 7);
            }

            THEN("the parsed severity class is correct") {
                REQUIRE(log_record.severity == logging::Severity::Trace);
            }

            THEN("the parsed message is correct") {
                REQUIRE(log_record.message == "trace log message");
            }
        }
    }

    GIVEN("an error log message") {
        const std::string msg = "2021-03-27 22:57:21 severity_test.cpp:func:14 ERROR: error occured";
        WHEN("the message is parsed") {
           auto log_record = logging::parse_log_message_to_record(msg); 

            THEN("the parsed date is correct") {
                REQUIRE(log_record.date.year == 2021);
                REQUIRE(log_record.date.month == 3);
                REQUIRE(log_record.date.day == 27);
            }

            THEN("the parsed time is correct") {
                REQUIRE(log_record.time.hour == 22);
                REQUIRE(log_record.time.minute == 57);
                REQUIRE(log_record.time.second == 21);
            }

            THEN("the parsed file information is correct") {
                REQUIRE(log_record.file_info.filename == "severity_test.cpp");
                REQUIRE(log_record.file_info.function_name == "func");
                REQUIRE(log_record.file_info.line_number == 14);
            }

            THEN("the parsed severity class is correct") {
                REQUIRE(log_record.severity == logging::Severity::Error);
            }

            THEN("the parsed message is correct") {
                REQUIRE(log_record.message == "error occured");
            }
        }
    }
}
