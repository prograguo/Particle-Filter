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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${str_hw3_SOURCE_DIR}/cmake")
find_package(LibConfig++ REQUIRED)
message("Libconfig++ found: ${LIBCONFIG_FOUND}")

find_package(SFML 2 REQUIRED system window graphics)

include_directories(
	${str_hw3_SOURCE_DIR}/include
	${Boost_INCLUDE_DIRS}
	${LIBCONFIGPP_INCLUDE_DIRS}
	${SFML_INCLUDE_DIRS})



################################################
## Test Executable to stream sensor data
################################################

add_executable(robot_viz_test
	src/robot_viz_test.cpp 
	src/helper_functions.cpp
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



add_executable(libconfig_example
	src/libconfig_example.cpp 
	)
target_link_libraries(libconfig_example
	${Boost_LIBRARIES}
	${LIBCONFIGPP_LIBRARIES}
	)

add_executable(sfml_example
	src/sfml_example.cpp
	src/grapher.cpp
	src/helper_functions.cpp
	)
target_link_libraries(sfml_example
	${Boost_LIBRARIES}
	${LIBCONFIGPP_LIBRARIES}
	${SFML_LIBRARIES}
	)

add_executable(motion_model_test
	src/test/motion_model_test.cpp
	src/grapher.cpp
	src/helper_functions.cpp
	src/str_motion_model.cpp
	)
target_link_libraries(motion_model_test
	${Boost_LIBRARIES}
	${LIBCONFIGPP_LIBRARIES}
	${SFML_LIBRARIES}
	)
