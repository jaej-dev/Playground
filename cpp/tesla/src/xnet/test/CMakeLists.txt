# Title   :   xnet test CMakeLists.txt
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required(VERSION 3.10)

project (xnet_test C CXX)

find_package (Threads)

add_executable (xnet_server
    xnet_server.cpp
    )

add_executable (xnet_client
    xnet_client.cpp
    )

set_target_properties (xnet_server xnet_client
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/test"
    COMPILE_FLAGS
    "${CMAKE_CXX_FLAGS} ${strict_cxx_flags}"
    )

add_definitions(-g)

include_directories (
  ${TESLA_OS_HEADER_PATH}
  ${TESLA_HEADER_PATH}
  )

link_directories(
  ${TESLA_OS_LIBS_PATH}
  ${TESLA_LIBS_PATH}
  )

target_link_libraries(xnet_server
    ${TESLA_DEPS_LIBS}
    ${TESLA_OS_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    )

target_link_libraries(xnet_client
    ${TESLA_DEPS_LIBS}
    ${TESLA_OS_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    )

install (
    TARGETS
    DESTINATION
    "${TESLA_TEST_PATH}"
    )
