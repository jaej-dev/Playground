# Title     :   The guiserver CMakeLists.txt
# Author    :   Jaewoong Jang
# Date      :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project(guiserver)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake"
    )

execute_process (
    COMMAND "${MODULE_PATH}/gen_src.py"
    ${CMAKE_SOURCE_DIR}
    ${MODULE_PATH}
    )

include (${MODULE_PATH}/gen_src.cmake)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

include_directories (
    ${GEN_INCLUDES}
    "${CMAKE_SOURCE_DIR}/../msgpack/include"
    "${CMAKE_SOURCE_DIR}/../common"
    "${CMAKE_SOURCE_DIR}/../common/data"
)

file(GLOB_RECURSE COMMON_HEADERS
    "${CMAKE_SOURCE_DIR}/../common/*.h"
    "${CMAKE_SOURCE_DIR}/../common/*.hpp"
)

file(GLOB_RECURSE COMMON_SOURCES
    "${CMAKE_SOURCE_DIR}/../common/*.c"
    "${CMAKE_SOURCE_DIR}/../common/*.cpp"
)

add_executable(${PROJECT_NAME}
    ${GEN_SOURCES}
    ${COMMON_HEADERS}
    ${COMMON_SOURCES}
)

install (
    TARGETS
    ${PROJECT_NAME}
    DESTINATION
    "${CMAKE_SOURCE_DIR}/../bin"
)
