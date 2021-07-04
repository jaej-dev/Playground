# Title   :   xlog CMakeLists
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project (xlog)

set (ENABLED_XLOG_DEBUG false)
set (ENABLED_XLOG_FILE_STREAM false)
set (ENABLED_XLOG_REMOTE_STREAM false)

set (PARAMS "\"")
set (XLOG_FILE_NAME "xlog.txt")
set (ENABLED_XLOG_FILE_PATH "${PARAMS}${DEF_TESLA_BUILD_DIR}/build/${XLOG_FILE_NAME}${PARAMS}")

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/chain_of_resp.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/chain_of_resp.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/singleton.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/singleton.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/severity.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/severity.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/time.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/time.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/record.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/record.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/base_stream.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/base_stream.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/file_stream.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/file_stream.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/remote_stream.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/remote_stream.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/console_stream.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/console_stream.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/xrecord.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/xrecord.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/xlog.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/xlog.h"
  )

add_subdirectory (test)
