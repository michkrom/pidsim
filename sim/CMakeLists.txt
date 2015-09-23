cmake_minimum_required(VERSION 2.8)

project (pidsimtest)

set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
		
add_subdirectory(lib)
	
add_executable( pidsimtest
	main.cpp
)

target_link_libraries( pidsimtest 
	pidsim
)
