# Title     :   The tesla CMakeLists.txt
# Author    :   Jaewoong Jang
# Date      :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project (tesla)

message(STATUS "\nThe tesla cmake start...\n")

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Build information
set (DEF_TESLA_VERSION_GIT "preliminary")
execute_process (
    COMMAND git describe --abbrev=4 --always --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE DEF_TESLA_VERSION_GIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
if ("${DEF_TESLA_VERSION_GIT}" STREQUAL "")
  set (DEF_TESLA_VERSION_GIT "preliminary")
endif()
message ("Git version: ${DEF_TESLA_VERSION_GIT}")
add_definitions ("-DDEF_TESLA_VERSION_GIT=\"${DEF_TESLA_VERSION_GIT}\"")

set (DEF_TESLA_BUILD_DIR "${CMAKE_SOURCE_DIR}")
set (DEF_TESLA_DEVELOPER_NAME "$ENV{USER}")
execute_process (
  COMMAND
  date
  OUTPUT_VARIABLE DEF_TESLA_BUILD_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Build directory: ${DEF_TESLA_BUILD_DIR}")
message(STATUS "Build machine user name: ${DEF_TESLA_DEVELOPER_NAME}")
message(STATUS "Build machine: ${CMAKE_SYSTEM_NAME}, processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Target machine: ${ARCH}, processor: ${PROC}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set (TESLA_LIBS_PATH "${CMAKE_SOURCE_DIR}/stage/lib/${ARCH}/${PROC}")
set (TESLA_HEADER_PATH "${CMAKE_SOURCE_DIR}/stage/lib/${ARCH}/${PROC}/include")
set (TESLA_BIN_PATH "${CMAKE_SOURCE_DIR}/stage/bin")
set (TESLA_ROOTFS_PATH "${CMAKE_SOURCE_DIR}/stage/rootfs")
set (TESLA_TEST_PATH "${CMAKE_SOURCE_DIR}/stage/test")
set (TESLA_CONFIG_PATH "${CMAKE_SOURCE_DIR}/stage/bin/config")
set (TESLA_CERTIFICATE_PATH "${CMAKE_SOURCE_DIR}/stage/certificate")
set (TESLA_CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message (STATUS "TESLA_LIBS_PATH: ${TESLA_LIBS_PATH}")
message (STATUS "TESLA_HEADER_PATH: ${TESLA_HEADER_PATH}")
message (STATUS "TESLA_BIN_PATH: ${TESLA_BIN_PATH}")
message (STATUS "TESLA_ROOTFS_PATH: ${TESLA_ROOTFS_PATH}")
message (STATUS "TESLA_TEST_PATH: ${TESLA_TEST_PATH}")
message (STATUS "TESLA_CONFIG_PATH: ${TESLA_CONFIG_PATH}")


if (${ARCH} MATCHES "Darwin")
  set (TESLA_OS_HEADER_PATH "/usr/local/opt/openssl/include")
  set (TESLA_OS_LIBS_PATH "/usr/local/opt/openssl/lib")
elseif (${ARCH} MATCHES "QNX")
  set (TESLA_OS_HEADER_PATH "/usr/local/opt/openssl/include")
  set (TESLA_OS_LIBS_PATH "/usr/local/opt/openssl/lib")
elseif (${ARCH} MATCHES "Linux")
  set (TESLA_OS_HEADER_PATH "/usr/local/opt/openssl/include")
  set (TESLA_OS_LIBS_PATH "/usr/local/opt/openssl/lib")
endif (${ARCH} MATCHES "Darwin")
message (STATUS "TESLA_OS_HEADER_PATH: ${OS_HEADER_PATH}")

# Generate architecture dependency
configure_file ("${CMAKE_SOURCE_DIR}/arch/arch_config.h.in"
                "${TESLA_HEADER_PATH}/arch_config.h")

# Static library
add_library(boost_system STATIC IMPORTED)
add_library(boost_date_time STATIC IMPORTED)
add_library(boost_iostreams STATIC IMPORTED)

set_target_properties (boost_system PROPERTIES
  IMPORTED_LOCATION 
  "${TESLA_LIBS_PATH}/libboost_system.a")

set_target_properties (boost_date_time PROPERTIES
  IMPORTED_LOCATION 
  "${TESLA_LIBS_PATH}/libboost_date_time.a")

set_target_properties (boost_iostreams PROPERTIES
  IMPORTED_LOCATION 
  "${TESLA_LIBS_PATH}/libboost_iostreams.a")

# Include header files
include_directories (
  ${TESLA_OS_HEADER_PATH}
  ${TESLA_HEADER_PATH}
  )

# Link directory
link_directories (
  ${TESLA_OS_LIBS_PATH}
  ${TESLA_LIBS_PATH}
  )

set (TESLA_DEPS_LIBS
  boost_system
  boost_date_time
  boost_iostreams
  )

message (STATUS "TESLA_DEPS_LIBS: ${TESLA_DEPS_LIBS}")

# Link library
if (${ARCH} MATCHES "Darwin")
  set (TESLA_OS_LIBS ${TESLA_OS_LIBS} pthread ssl crypto)
elseif (${ARCH} MATCHES "QNX")
  set (TESLA_OS_LIBS ${TESLA_OS_LIBS} socket m ssl crypto)
elseif (${ARCH} MATCHES "Linux")
  set (TESLA_OS_LIBS ${TESLA_OS_LIBS} rt pthread anl ssl crypto)
endif (${ARCH} MATCHES "Darwin")

message (STATUS "TESLA_OS_LIBS: ${TESLA_OS_LIBS}")

# Generate cetificate
execute_process (
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/./gen_certificate.sh"
    )
# Source 
add_subdirectory (src)


message(STATUS "\nThe tesla cmake finished...\n")
