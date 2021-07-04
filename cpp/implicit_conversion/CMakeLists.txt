# Title   :   implicit_conversion CMakeLists.txt
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project (implicit_conversion C CXX)

find_package (Threads)

# Target architecture
if (NOT ARCH)
  set (ARCH ${CMAKE_SYSTEM_NAME})
endif (NOT ARCH)

if (NOT PROC)
  set (PROC ${CMAKE_SYSTEM_PROCESSOR})
endif (NOT PROC)

if (${PROC} MATCHES "x86$")
  message (FATAL_ERROR "Does not supported ${PROC} processor.")
elseif (${PROC} MATCHES "x86_64$")
  set (PROC_X86_64 ON)
endif (${PROC} MATCHES "x86$")

# Compile option
if (${ARCH} MATCHES "Darwin")
  set (ARCH_DARWIN ON)
  set (CMAKE_CXX_FLAGS
       "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -fno-strict-aliasing -Wno-deprecated")
elseif (${ARCH} MATCHES "QNX")
  set (ARCH_QNX ON)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fno-strict-aliasing")
elseif (${ARCH} MATCHES "Linux")
  set (ARCH_LINUX ON)
  set (CMAKE_CXX_FLAGS
       "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -no-pie -fno-strict-aliasing -Wno-deprecated")
endif (${ARCH} MATCHES "Darwin")

# Source
set (implicit_conversion_SOURCES
    implicit_conversion.cpp
    )

add_executable (implicit_conversion
    ${implicit_conversion_SOURCES}
    )

set_target_properties (implicit_conversion
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}"
    COMPILE_FLAGS
    "${CMAKE_CXX_FLAGS} ${strict_cxx_flags}"
    )

add_definitions(-g)

target_link_libraries( 
    implicit_conversion
    ${CMAKE_THREAD_LIBS_INIT}
    )

