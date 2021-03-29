#include <thread>

#include <catch2/catch.hpp>
#include <cpplog/log.hpp>

SCENARIO("Sync_Ostream can be used across threads", "[Sync_Ostream]") {
    GIVEN("a synced line out stream backed with a string stream") {
        std::stringstream ss{};
        logging::Sync_Ostream sync_ss{ss};

        WHEN("two threads write to the synced stream") {
            std::thread t1{[&sync_ss]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                sync_ss << "thread" << 1;
            }};
            std::thread t2{[&sync_ss]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                sync_ss << "thread" << 2;
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
