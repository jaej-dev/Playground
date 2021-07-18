cmake_minimum_required (VERSION 3.6.3)

project(poco_udp_client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(poco_udp_client
  poco_udp_client.cpp
  )

target_include_directories(poco_udp_client 
  PRIVATE "${POCO_INCLUDE_DIR}"
  "${TESTBED_POCO_INCLUDE_DIR}"
  )

target_link_libraries(poco_udp_client 
  "${POCO_LIBS}"
  "${TESTBED_POCO_LIBS}"
  )

set_target_properties (poco_udp_client 
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}"
  COMPILE_FLAGS
  "${CMAKE_CXX_FLAGS}"
  )
