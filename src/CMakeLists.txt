add_library(
    ${PROJECT_NAME}
    SHARED
    log_parser.cpp
    log.cpp
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_include_directories(${PROJECT_NAME} PRIVATE ./)

# All users of this library will need at least C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Install the target and create export-set
include(GNUInstallDirs)
set(PROJECT_TARGETS "${PROJECT_NAME}Targets")
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_TARGETS}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header file
install(
    FILES 
    "${PROJECT_INCLUDE_DIR}/cpplog/log.hpp"
    "${PROJECT_INCLUDE_DIR}/cpplog/log_parser.hpp"
    DESTINATION 
    ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(
    EXPORT ${PROJECT_TARGETS}
    FILE "${PROJECT_NAME}${PROJECT_TARGETS}.cmake"
    NAMESPACE "${PROJECT_NAME}::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
