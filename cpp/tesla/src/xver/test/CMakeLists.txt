# Title   :   xver_test CMakeLists
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required(VERSION 3.10)

project (xver_test C CXX)

set (xver_test_SOURCES
    xver_test.cpp
    )

add_executable (xver_test
    ${xver_test_SOURCES}
    )

set_target_properties (xver_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/test"
    COMPILE_FLAGS
    "${CMAKE_CXX_FLAGS} ${strict_cxx_flags}"
    )

add_definitions(-g)

target_link_libraries( 
    xver_test
    ${TESLA_DEPS_LIBS}
    ${TESLA_OS_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    )

install (
    TARGETS
    xver_test
    DESTINATION
    "${TESLA_TEST_PATH}"
    )
