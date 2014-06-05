cmake_minimum_required(VERSION 2.8)
project( pepe )

include_directories(include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable( intestazione 
	src/Intestazione.cpp 
	)
	
target_link_libraries( intestazione 
	owlsock 
	)