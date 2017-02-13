#Check cmake version
cmake_minimum_required(VERSION 3.0)

project(cppTest CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(
  -std=c++11
  -Wall
  -finstrument-functions
  )

add_executable(
  ${PROJECT_NAME}.run
  src/cppTest.cpp
  )

