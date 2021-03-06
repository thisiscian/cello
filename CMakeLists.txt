cmake_minimum_required (VERSION 2.8)
project (cello)
set (VERSION_MAJOR 0)
set (VERSION_MINOR 0)
set (TITLE "${PROJECT_NAME}")
set (CMAKE_CXX_FLAGS "-O2 -g")
set (TESTDIR "${PROJECT_SOURCE_DIR}/test")

enable_testing()

configure_file (
  "${PROJECT_SOURCE_DIR}/configurable/version.h.in"
  "${PROJECT_SOURCE_DIR}/include/cello/version.h"
)

include_directories("${PROJECT_SOURCE_DIR}/include/")

add_library(
	${PROJECT_NAME}
	src/frame.cpp
	src/pixel.cpp
	src/image.cpp
	src/colourMap.cpp
	src/cello.cpp
)
	
target_link_libraries(${PROJECT_NAME} gif)

macro(add_animation arg) 
	add_executable (
		${arg}
		animation/${arg}.cpp
	)
	target_link_libraries(${arg} ${PROJECT_NAME})
	add_test(check_standard_${arg} ${TESTDIR}/check_standard ${arg})
	add_test(check_custom_${arg} ${TESTDIR}/check_custom ${arg})
endmacro(add_animation)

add_animation(test1)
add_animation(test2)

