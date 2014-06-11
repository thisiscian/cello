cmake_minimum_required (VERSION 2.8)
project (cello)
set (VERSION_MAJOR 0)
set (VERSION_MINOR 0)
set (TITLE "${PROJECT_NAME}")
set (CMAKE_CXX_FLAGS "-O2")

configure_file (
  "${PROJECT_SOURCE_DIR}/configurable/version.h.in"
  "${PROJECT_SOURCE_DIR}/include/cello/version.h"
)

include_directories("${PROJECT_SOURCE_DIR}/include/")

add_library(
	${PROJECT_NAME}
	src/image.cpp
	src/frame.cpp
	src/gif.cpp
	src/pixel.cpp
)
	
target_link_libraries(${PROJECT_NAME} boost_program_options gif)

macro(add_animation arg) 
	add_executable (
		${arg}
		animation/${arg}.cpp
	)
	target_link_libraries(${arg} ${PROJECT_NAME})
endmacro(add_animation)

add_animation(test2)
