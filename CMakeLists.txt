cmake_minimum_required(VERSION 2.8)
project( Phase_Space_Acquisition )
set(CMAKE_CXX_FLAGS "-std=c++0x")

include_directories(include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable( Phase_Space  
	src/Phase_Space.cpp 
	)
	
target_link_libraries( Phase_Space  
	owlsock 
	)