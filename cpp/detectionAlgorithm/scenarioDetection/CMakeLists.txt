# requires minimum cmake version
cmake_minimum_required(VERSION 2.8.9)

# produce the cmake var PROJECT_NAME
project(multiObjects)

# load LSLAppBoilerplate if not done already
if(NOT TARGET LSL::lsl)
    if(NOT LSL_INSTALL_ROOT)
        # If building the app out of tree, the user is expected to supply LSL_INSTALL_ROOT.
        # This default directory is may be wrong for you. Ignore it.
        # Just supply -DLSL_INSTALL_ROOT=/path/to/installed/lsl to the cmake command.
        set(LSL_INSTALL_ROOT "/home/nduarte/Documents/pupil/labstreaminglayer")
        set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../../build/install/lsl_Release")
        get_filename_component(LSL_INSTALL_ROOT ${LSL_INSTALL_ROOT} ABSOLUTE)
        if(NOT EXISTS ${LSL_INSTALL_ROOT})
            message(FATAL_ERROR "When building a single app out of tree you need to set LSL_INSTALL_ROOT ( cmake -DLSL_INSTALL_ROOT=/path/to/installed/lsl)")
        endif()
    endif()
    if(NOT IS_ABSOLUTE ${LSL_INSTALL_ROOT})
        message(FATAL_ERROR "LSL_INSTALL_ROOT needs to be an absolute path")
    endif()
    file(TO_CMAKE_PATH ${LSL_INSTALL_ROOT} LSL_INSTALL_ROOT)
    list(APPEND CMAKE_MODULE_PATH "${LSL_INSTALL_ROOT}/cmake")
    include(LSLAppBoilerplate)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
    include(LSLSaneDefaults)
endif()

# convenience function to add an example file
# this creates a target, links the necessary libraries and
# creates an install target
#function(addlslexample name extension)
#	add_executable(${name}
#		${name}.${extension}
#	)
#	target_link_libraries(${name} PRIVATE LSL::lsl)
#	target_compile_features(${name} PRIVATE cxx_constexpr)
#	installLSLApp(${name})
#endfunction()

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
          
# this creates a target, links the necessary libraries and creates an install target for LSL packages
target_link_libraries(${PROJECT_NAME} PRIVATE LSL::lsl)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_constexpr)
installLSLApp(${PROJECT_NAME})

                                    
