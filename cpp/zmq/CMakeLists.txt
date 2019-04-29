cmake_minimum_required(VERSION 3.5)

project(cppzmq VERSION 1.0.0 LANGUAGES CXX)

find_package(cppzmq)
#find_package(OpenCV)
find_package(msgpack REQUIRED)
include_directories(/home/nduarte/software/msgpack-c/build/INSTALL/include)
#include_directories(/home/nduarte/software/opencv-3.4.1/build_w_atabak/include)
include_directories(/home/nduarte/software/opencv-3.4.1/build/INSTALL/include/)
include_directories(/usr/local/cuda-8.0/include)

#find_package( OpenCV REQUIRED )

#find_package( OpenCV REQUIRED )
#set(OpenCV_DIR /home/nduarte/software/opencv-3.4.1/build)
set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda-8.0)
find_package(OpenCV 3.4.1 EXACT REQUIRED PATHS /home/nduarte/software/opencv-3.4.1/build/INSTALL/share/OpenCV/ NO_DEFAULT_PATH)


#if( OpenCV_FOUND )
#list( APPEND ThirdParty_LIBS ${OpenCV_LIBS} )
#    include_directories( ${OpenCV_INCLUDE_DIRS} )
#endif( OpenCV_FOUND )

enable_testing()
add_executable(
    sub #demo
    subscriber.cpp
    )

target_link_libraries(
    sub
    cppzmq
    ${OpenCV_LIBS}
    )

enable_testing()
add_executable(
    demo
    test.cpp
    )

target_link_libraries(
    demo
    cppzmq
    ${OpenCV_LIBS}
    )

#target_compile_options(sub PRIVATE -Werror)
#target_compile_features(sub PRIVATE cxx_std_11)

find_package( Boost REQUIRED COMPONENTS system thread)
include_directories( ${Boost_INCLUDE_DIRS} )
add_executable(pub publisher.cpp)
target_link_libraries(
    pub
    ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} 
    cppzmq
    )


add_test(
  NAME
    sub
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/sub
  )

add_test(
  NAME
    pub
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/pub
  )

add_test(
  NAME
    demo
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test
  )
