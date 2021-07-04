# Title   :   xcfg_test CMakeLists
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required(VERSION 3.10)

project (xcfg_test C CXX)

set (xcfg_test_SOURCES
    xcfg_test.cpp
    )

add_executable (xcfg_test
    ${xcfg_test_SOURCES}
    )

set_target_properties (xcfg_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/test"
    COMPILE_FLAGS
    "${CMAKE_CXX_FLAGS} ${strict_cxx_flags}"
    )

add_definitions(-g)

target_link_libraries( 
    xcfg_test
    ${TESLA_DEPS_LIBS}
    ${TESLA_OS_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    )

install (
    TARGETS
    xcfg_test
    DESTINATION
    "${TESLA_TEST_PATH}"
    )
