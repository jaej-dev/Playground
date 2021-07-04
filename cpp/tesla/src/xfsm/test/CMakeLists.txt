# Title   :   xfsm test CMakeLists.txt
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required(VERSION 3.10)

project (xfsm_test C CXX)

find_package (Threads)

set (xfsm_test_SOURCES
    xfsm_test.cpp
    master.cpp
    slave.cpp
    )

add_executable (xfsm_test
    ${xfsm_test_SOURCES}
    )

set_target_properties (xfsm_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/test"
    COMPILE_FLAGS
    "${CMAKE_CXX_FLAGS} ${strict_cxx_flags}"
    )

add_definitions(-g)

target_link_libraries( 
    xfsm_test
    ${TESLA_DEPS_LIBS}
    ${TESLA_OS_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    )

install (
    TARGETS
    xfsm_test
    DESTINATION
    "${TESLA_TEST_PATH}"
    )
