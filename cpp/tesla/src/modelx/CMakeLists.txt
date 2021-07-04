# Title   :   modelx CMakeLists.txt
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

project (modelx)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Generate source file list
execute_process (
    COMMAND "${CMAKE_MODULE_PATH}/gen_src.py"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_MODULE_PATH}
    )

include (${CMAKE_MODULE_PATH}/gen_src.cmake)

add_definitions(-DTARGET_MODELS)

include_directories (
  ${GEN_INCLUDES}
  ${TESLA_HEADER_PATH}
  )

link_directories (
  ${TESLA_LIB_PATH}
  )

set (modelx_SOURCES
    ${GEN_SOURCES}
    )

add_executable (modelx
  ${modelx_SOURCES}
  )

target_link_libraries (modelx
  ${TESLA_DEPS_LIBS}
  ${TESLA_OS_LIBS}
  #ssl
  #crypto
  #rpcserver
  #socket
  )

set_target_properties (modelx
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}"
  COMPILE_FLAGS
  "${strict_cxx_flags}"
  )

# Configuration files deployment 
include (${CMAKE_MODULE_PATH}/deployment.cmake)

file(GLOB INSTALL_TESLA_XML
  "${CMAKE_SOURCE_DIR}/config/*.xml"
  "${CMAKE_SOURCE_DIR}/config/*.xsd"
)

list (APPEND deployment_list
  ${INSTALL_TESLA_XML}
  )

add_deployment_file_list("${deployment_list}" "/")

# Install
install (
  TARGETS 
  modelx
  DESTINATION
  "${TESLA_BIN_PATH}"
)

install (
  FILES
  ${INSTALL_TESLA_XML}
  DESTINATION
  "${TESLA_CONFIG_PATH}"
)


