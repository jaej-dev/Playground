cmake_minimum_required (VERSION 3.6.3)

project(testbed_boost)

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
  set (CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} -m32 -std=c++11 -g -O2 -no-pie -fno-strict-aliasing -Wno-deprecated"
    )
elseif (${PROC} MATCHES "x86_64$")
  set (CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -no-pie -fno-strict-aliasing -Wno-deprecated"
    )
endif (${PROC} MATCHES "x86$")

# Boost library
set(BOOST_PREFIX "${CMAKE_SOURCE_DIR}/stage/${ARCH}/${PROC}")
set(BOOST_INCLUDE_DIR "${BOOST_PREFIX}/include")
set(BOOST_LIB_DIR "${BOOST_PREFIX}/lib")

add_library(boost_system STATIC IMPORTED)
add_library(boost_date_time STATIC IMPORTED)
add_library(boost_iostreams STATIC IMPORTED)

set_target_properties (boost_system PROPERTIES
  IMPORTED_LOCATION 
  "${BOOST_LIB_DIR}/libboost_system.a")

set_target_properties (boost_date_time PROPERTIES
  IMPORTED_LOCATION 
  "${BOOST_LIB_DIR}/libboost_date_time.a")

set_target_properties (boost_iostreams PROPERTIES
  IMPORTED_LOCATION 
  "${BOOST_LIB_DIR}/libboost_iostreams.a")

set(BOOST_LIBS 
  boost_system
  boost_date_time
  boost_iostreams
  )

# Internal and OS library
set(TESTBED_BOOST_PREFIX "${CMAKE_SOURCE_DIR}/stage/${ARCH}/${PROC}")
set(TESTBED_BOOST_INCLUDE_DIR "${TESTBED_BOOST_PREFIX}/include")
set(TESTBED_BOOST_LIB_DIR "${TESTBED_BOOST_PREFIX}/lib")
set(TESTBED_BOOST_LIBS ${TESTBED_BOOST_LIBS} pthread)

add_subdirectory(src)
