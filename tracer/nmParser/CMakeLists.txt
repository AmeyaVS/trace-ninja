#check cmake version
cmake_minimum_required(VERSION 3.0)

project(nmParser CXX)

find_package(Boost REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  ${Boost_INCLUDE_DIR}
  )

add_definitions(
  -std=c++11
  -Wall
  )

add_executable(
  ${PROJECT_NAME}.run
  src/nmParser.cpp
  )

add_executable(
  mapParser.run
  src/mapParser.cpp
  )
