# requires minimum cmake version
cmake_minimum_required(VERSION 2.8.9)

# produce the cmake var PROJECT_NAME
project(videoCamera)

# import math symbols from standard cmath
add_definitions(-D_USE_MATH_DEFINES)

find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
list( APPEND ThirdParty_LIBS ${OpenCV_LIBS} )
    include_directories( ${OpenCV_INCLUDE_DIRS} )
endif( OpenCV_FOUND )

# follower
add_executable(${PROJECT_NAME} 
		               	${CMAKE_SOURCE_DIR}/videoCamera.cpp
				)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

