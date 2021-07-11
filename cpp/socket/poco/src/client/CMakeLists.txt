cmake_minimum_required (VERSION 3.6.3)

project(client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(client
  client.cpp
  )

target_include_directories(client 
  PRIVATE "${POCO_INCLUDE_DIR}"
  "${TESTBED_INCLUDE_DIR}"
  )

target_link_libraries(client 
  "${POCO_LIBS}"
  "${TESTBED_LIBS}"
  )

set_target_properties (client 
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}"
  COMPILE_FLAGS
  "${CMAKE_CXX_FLAGS}"
  )
