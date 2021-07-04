# Title   :   tesla source code CMakeLists.txt
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project (tesla_src)

add_subdirectory (modelx)
add_subdirectory (xfsm)
add_subdirectory (xnet)
add_subdirectory (xver)
add_subdirectory (xcfg)
add_subdirectory (xlog)
