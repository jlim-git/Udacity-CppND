cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(RMS)

configure_file(src/sampleMenu.txt sampleMenu.txt COPYONLY)

file(GLOB project_SRCS src/*.cpp) #src/*.h

add_executable(RMS ${project_SRCS})
