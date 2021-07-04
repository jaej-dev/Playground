# Title   :   xlog test CMakeLists.txt
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required(VERSION 3.10)

project (xlog_test C CXX)

find_package (Threads)

set (xlog_test_SOURCES
    xlog_test.cpp
    )

add_executable (xlog_test
    ${xlog_test_SOURCES}
    )

set_target_properties (xlog_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/test"
    COMPILE_FLAGS
    "${CMAKE_CXX_FLAGS} ${strict_cxx_flags}"
    )

add_definitions(-g)

target_link_libraries( 
    xlog_test
    ${TESLA_DEPS_LIBS}
    ${TESLA_OS_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    )

install (
    TARGETS
    xlog_test
    DESTINATION
    "${TESLA_TEST_PATH}"
    )
