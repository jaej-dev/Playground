cmake_minimum_required (VERSION 3.6.3)

project(HelloWorld)

set(CMAKE_CXX_STANDARD 11)

add_executable(HelloWorld main.cpp)

set(POCO_PREFIX "/usr/local") # the directory containing "include" and "lib"
set(POCO_INCLUDE_DIR "${POCO_PREFIX}/include")
set(POCO_LIB_DIR "${POCO_PREFIX}/lib")
set(POCO_LIBS "${POCO_LIB_DIR}/libPocoFoundation.so")


target_include_directories(HelloWorld PRIVATE "${POCO_INCLUDE_DIR}")
target_link_libraries(HelloWorld "${POCO_LIBS}")
