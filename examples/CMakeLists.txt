set(EXAMPLES_EXEC "examples_lib")

add_executable(
    ${EXAMPLES_EXEC}
    basic.cpp
)

target_compile_features(${EXAMPLES_EXEC} PRIVATE cxx_std_17)

# Link to main lib
target_link_libraries(${EXAMPLES_EXEC} PRIVATE ${CMAKE_PROJECT_NAME})
