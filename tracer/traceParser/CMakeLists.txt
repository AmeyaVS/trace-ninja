#Check cmake version
cmake_minimum_required(VERSION 3.0)

project(traceParser CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)

add_definitions(
  -std=c++11
  -Wall
  )

include_directories(
  $ENV{HOME}/apps/tracer/include
  ${Boost_INCLUDE_DIR}
  include
  )

add_executable(
  ${PROJECT_NAME}.run
  src/traceParser.cpp
  src/utils.cpp
  src/nmParser.cpp
  src/mapParser.cpp
  )

install(
  TARGETS ${PROJECT_NAME}.run DESTINATION $ENV{HOME}/apps/tracer/bin
  )
