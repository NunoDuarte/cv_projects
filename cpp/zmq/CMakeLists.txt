cmake_minimum_required(VERSION 3.5)

project(cppzmq VERSION 1.0.0 LANGUAGES CXX)

find_package(cppzmq)
find_package(msgpack REQUIRED)
include_directories(/home/nduarte/software/msgpack-c/build/INSTALL/include)
include_directories(/home/nduarte/software/opencv-3.4.1/build/INSTALL/include/)
include_directories(/usr/local/cuda-8.0/include)

#set(OpenCV_DIR /home/nduarte/software/opencv-3.4.1/build)
set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda-8.0)
find_package(OpenCV 3.4.1 EXACT REQUIRED PATHS /home/nduarte/software/opencv-3.4.1/build/INSTALL/share/OpenCV/ NO_DEFAULT_PATH)

enable_testing()
add_executable(
    sub 
    subscriber.cpp
    )

target_link_libraries(
    sub
    cppzmq
    ${OpenCV_LIBS}
    )

#target_compile_options(sub PRIVATE -Werror)
#target_compile_features(sub PRIVATE cxx_std_11)

find_package( Boost REQUIRED COMPONENTS system thread)
include_directories( ${Boost_INCLUDE_DIRS} )

add_test(
  NAME
    sub
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/sub
  )


