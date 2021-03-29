#include <catch2/catch.hpp>
#include <cpplog/log.hpp>

SCENARIO("Messages send to the error log print with prefix", "[Error]") {
    GIVEN("") {
        std::stringstream ss{}
        log::AddBackend(ss, log::Severity.Error);

        WHEN("messages are send to the error log") {
            std::string error_msg = "error occured";
            ERROR << error_msg << std::endl;

            THEN("the message is send to the the stream with the prefix") {
                REQUIRE(ss.str() == ("[Errror]" + error_msg));
            }
        }
    }
}

