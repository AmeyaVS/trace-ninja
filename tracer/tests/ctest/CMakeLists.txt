#Check cmake version
cmake_minimum_required(VERSION 3.0)

project(ctest C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(
  -Wall
  -finstrument-functions
  )

add_executable(
  ${PROJECT_NAME}.run
  src/ctest.c
  )

