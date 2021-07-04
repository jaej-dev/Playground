# Title   :   xnet CMakeLists.txt
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project (xnet)

set (ENABLED_DEBUG_XNET_SESSION true)
set (ENABLED_DEBUG_XNET_SERVER true)
set (ENABLED_DEBUG_XNET_CLIENT true)


configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/session.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/session.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/server.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/server.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/client.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/client.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/logger_adaptor.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/logger_adaptor.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/singleton.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/singleton.h"
  )

#install (
#  TARGETS
#  xnet
#  DESTINATION
#  "${TESLA_LIB_PATH}"

add_subdirectory (test)
