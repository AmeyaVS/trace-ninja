#Check cmake version
cmake_minimum_required(VERSION 3.0)

project(tracer C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-Wall)

include_directories(
  include
  )

add_library(
  ${PROJECT_NAME} SHARED
  src/tracer.c
  )

install(
  TARGETS ${PROJECT_NAME} DESTINATION $ENV{HOME}/apps/tracer/lib
  )
install(
  FILES include/tracer.h DESTINATION $ENV{HOME}/apps/tracer/include
  )
