# Title   :   xcfg CMakeLists
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project (xcfg)

include_directories (
  ${CAPRI_SRC_PATH}
  ${TESLA_HEADER_PATH}
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/xcfg.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/xcfg.h"
  )

add_subdirectory (test)
