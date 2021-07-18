cmake_minimum_required (VERSION 3.6.3)

project(testbed_poco)

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

set(POCO_PREFIX "/usr/local")
set(POCO_INCLUDE_DIR "${POCO_PREFIX}/include")
set(POCO_LIB_DIR "${POCO_PREFIX}/lib")
set(POCO_LIBS 
  "${POCO_LIB_DIR}/libPocoFoundation.so"
  "${POCO_LIB_DIR}/libPocoNet.so"
  )

set(TESTBED_POCO_PREFIX "${CMAKE_SOURCE_DIR}/stage/lib/${ARCH}/${PROC}")
set(TESTBED_POCO_INCLUDE_DIR "${TESTBED_POCO_PREFIX}/include")
set(TESTBED_POCO_LIB_DIR "${TESTBED_POCO_PREFIX}/lib")
set(TESTBED_POCO_LIBS ${TESTBED_POCO_LIBS} pthread)

add_subdirectory(src)
