cmake_minimum_required(VERSION 2.8)

project (pidsim)

set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(
	pidsim 
	STATIC 
	src/pidsim.cpp)
