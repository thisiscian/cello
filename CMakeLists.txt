cmake_minimum_required (VERSION 2.8)
project (cello)
set (VERSION_MAJOR 0)
set (VERSION_MINOR 0)
set (TITLE "${PROJECT_NAME}")
set (CMAKE_CXX_FLAGS "-O2 -lboost_program_options -lgif")

configure_file (
  "${PROJECT_SOURCE_DIR}/configurable/version.h.in"
  "${PROJECT_SOURCE_DIR}/include/cello/version.h"
)

include_directories("${PROJECT_SOURCE_DIR}/include/")

add_executable(
	${PROJECT_NAME}
	src/main.cpp
	src/settings.cpp
	src/pixel.cpp
	src/frame.cpp
	src/animation.cpp
)
