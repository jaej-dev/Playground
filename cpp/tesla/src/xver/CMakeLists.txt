# Title   :   xver CMakeLists
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project (xver)

set (DEF_TESLA_VERSION_MAJOR 1)
set (DEF_TESLA_VERSION_MINOR 0)
set (DEF_TESLA_VERSION_PATCH 0)
set (DEF_TESLA_VERSION_STRING "${DEF_TESLA_VERSION_MAJOR}.${DEF_TESLA_VERSION_MINOR}.${DEF_TESLA_VERSION_PATCH}-${DEF_TESLA_VERSION_GIT}")
set (DEF_TESLA_FEATURES
  "xmsg"
  "xver"
  "capri"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/xver.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/xver.h"
  )

add_subdirectory (test)
