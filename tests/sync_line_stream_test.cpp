#include <tuple>
#include <vector>
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>

#include <catch2/catch.hpp>
#include <cpplog/log.hpp>

/*
SCENARIO("Sync_Ostream defaults to flushing the stream on std::endl", "[Sync_Ostream]") {
    GIVEN("a synced line out stream backed with a string stream") {
        std::stringstream ss{};
        log::Sync_Ostream sync_ss{ss};

        WHEN("messages are send to the stream without a newline") {
            sync_ss << "message body";
            THEN("nothing is printed") {
                REQUIRE(ss.str() == "");
            }
        }
        WHEN("messages are send to the stream with a newline") {
            std::string msg_body = "message body";
            sync_ss << msg_body << std::endl;
            THEN("the message is printed with a newline") {
                REQUIRE(ss.str() == (msg_body + "\n"));
            }
        }
    }
}

SCENARIO("Sync_Ostream can have its' prefix changed in a thread safe way", "[Sync_Ostream]") {
    GIVEN("a synced line out stream backed with a string stream") {
        std::stringstream ss{};
        log::Sync_Ostream sync_ss{ss};

        WHEN("the default prefix is used") {
            THEN("no prefix is printed") {
                std::string msg_body{"message body"};
                sync_ss << msg_body << std::endl;
                REQUIRE(ss.str() == msg_body + "\n");
            }
        }

        WHEN("the prefix is set in the constructor") {
            std::string prefix = "TEST_PREFIX";
            log::Sync_Ostream sync_prefixed_ss{ss, prefix};

            THEN("the prefix is printed to the ostream") {
                std::string msg_body{"message body"};
                sync_prefixed_ss << msg_body << std::endl;
                REQUIRE(ss.str() == (prefix + msg_body + "\n"));
            }
        }
    }
}*/

SCENARIO("Sync_Ostream can be used across threads", "[Sync_Ostream]") {
    GIVEN("a synced stream backed with a string stream") {
        std::stringstream ss{};
        log::Sync_Ostream sync_ss{ss};

        WHEN("two threads write to the synced stream") {
            std::thread t1{[&sync_ss]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                sync_ss << "thread1";
            }};
            std::thread t2{[&sync_ss]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                sync_ss << "thread2";
            }};
            t1.join();
            t2.join();
            sync_ss << std::endl;

            THEN("both writes from the threads are written atomically") {
                REQUIRE((ss.str() == "thread1thread2\n" || ss.str() == "thread2thread1\n"));
            }
        }
    }
}
