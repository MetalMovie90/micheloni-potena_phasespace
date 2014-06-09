cmake_minimum_required(VERSION 2.8)
project( Ransac )

find_package(
    OpenCV
    REQUIRED 
    )

include_directories(
    include
    )

add_executable( RANSAC  
	src/Ransac.cpp 
	)
	

target_link_libraries( 
    RANSAC 
    ${OpenCV_LIBS} 
    )