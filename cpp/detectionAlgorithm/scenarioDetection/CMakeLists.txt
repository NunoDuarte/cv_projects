# requires minimum cmake version
cmake_minimum_required(VERSION 2.8.9)

# produce the cmake var PROJECT_NAME
project(multiObjects)

if(NOT TARGET LSL::lsl)
	# when building out of tree LSL_ROOT needs to be specified on the cmd line
	list(APPEND LSL_INSTALL_ROOT "/home/nduarte/Documents/pupil/labstreaminglayer/build/install")
	find_package(LSL HINTS ${LSL_INSTALL_ROOT}/lsl_Release/LSL ${LSL_INSTALL_ROOT}/lsl_Release/LSL QUIET)
	if(NOT LSL_FOUND)
		message(FATAL_ERROR "Precompiled LSL was not found. Set LSL_INSTALL_ROOT to the LSL installation path ( cmake -DLSL_INSTALL_ROOT=/path/to/installed/lsl)")
	endif()
	list(APPEND CMAKE_MODULE_PATH ${LSL_DIR})
	message(STATUS "Looking for LSLCMake in ${LSL_DIR}")
	include(LSLCMake)
endif()

find_package( Threads )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
list( APPEND ThirdParty_LIBS ${OpenCV_LIBS} )
    include_directories( ${OpenCV_INCLUDE_DIRS} )
endif( OpenCV_FOUND )

if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function")
endif()
file(GLOB srcs ./*.cpp ./*.h*)
if(NOT HAVE_OPENGL)
  #ocv_list_filterout(all_samples "opengl")
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

#set(OPENGL_LIBRARIES "/usr/include/GL")
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

# executable
add_executable(${PROJECT_NAME} 
		               	${CMAKE_SOURCE_DIR}/multithreading.cpp
				)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} )

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
                                              
