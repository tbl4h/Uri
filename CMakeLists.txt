cmake_minimum_required(VERSION 3.8)
#set(CMAKE_C_COMPILER "/usr/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++-9")
set(This Uri)
project(${This} C CXX)


set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
enable_testing()

add_subdirectory(googletest)
add_subdirectory(test)
file(GLOB_RECURSE fileCollection "./*.cpp")
set(Headers 
    Uri.hpp
    )
set(Sources 
    Uri.cpp
    )

add_library(${This} STATIC ${Sources} ${Headers})

