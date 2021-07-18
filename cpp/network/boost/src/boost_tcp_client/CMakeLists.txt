cmake_minimum_required (VERSION 3.6.3)

project(boost_udp_client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(boost_udp_client
  client.cpp
  )

target_include_directories(boost_udp_client 
  PRIVATE "${BOOST_INCLUDE_DIR}"
  "${TESTBED_BOOST_INCLUDE_DIR}"
  )

target_link_libraries(boost_udp_client 
  "${BOOST_LIBS}"
  "${TESTBED_BOOST_LIBS}"
  )

set_target_properties (boost_udp_client 
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}"
  COMPILE_FLAGS
  "${CMAKE_CXX_FLAGS}"
  )
