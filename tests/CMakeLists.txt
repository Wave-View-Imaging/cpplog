include(FetchContent)

# Testing library
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.5
  )

# Adds Catch2::Catch2
FetchContent_MakeAvailable(Catch2)

set(TEST_EXEC "test_lib")

add_executable(
    ${TEST_EXEC}
    test_lib.cpp
    sync_stream_test.cpp
    parser_test.cpp
    severity_test.cpp
)

target_compile_features(${TEST_EXEC} PRIVATE cxx_std_17)

target_compile_options(${TEST_EXEC} PRIVATE "-Wmacro-redefined")

# Link to main lib and catch2 
target_link_libraries(${TEST_EXEC} PRIVATE ${CMAKE_PROJECT_NAME} Catch2::Catch2)

# Register tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)
catch_discover_tests(${TEST_EXEC})
