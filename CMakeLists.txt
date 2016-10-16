cmake_minimum_required(VERSION 2.8)

project(str_hw3)
message("project root is: ${str_hw3_SOURCE_DIR}")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${str_hw3_SOURCE_DIR}/bin)
message("Executables saved to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

find_package(Boost REQUIRED COMPONENTS 
	system
	filesystem
	iostreams)
message("Boost found: ${Boost_FOUND}")

include_directories(
	${str_hw3_SOURCE_DIR}/include
	${Boost_INCLUDE_DIRS})



################################################
## Test Executable to stream sensor data
################################################

add_executable(robot_viz_test
	src/robot_viz_test.cpp 
	#include/gnuplot-iostream.h 
	)
target_link_libraries(robot_viz_test
	${Boost_LIBRARIES}
	)
add_executable(test_ray_tracer
	src/test_ray_tracer.cpp
	)
target_link_libraries(test_ray_tracer
	${Boost_LIBRARIES}
	)


################################################
## Example executeable for gnuplot-iostream test
################################################
add_executable(example-data-2d
	src/example-data-2d.cpp
	include/gnuplot-iostream.h  
	)
target_link_libraries(example-data-2d 
	${Boost_LIBRARIES}
	)