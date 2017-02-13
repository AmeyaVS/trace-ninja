#Check cmake version
cmake_minimum_required(VERSION 3.0)

project(chrono CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(
  -std=c++11
  -Wall
  -finstrument-functions
  #-finstrument-functions-exclude-file-list='/usr/include/c++/5/chrono'
  -finstrument-functions-exclude-file-list='/usr/include/c++/5/chrono'
  )

add_executable(
  ${PROJECT_NAME}.run
  src/chrono.cpp
  )

