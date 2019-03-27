cmake_minimum_required(VERSION 3.5)

project(cppzmq VERSION 1.0.0 LANGUAGES CXX)

find_package(cppzmq)
find_package(msgpack REQUIRED)
include_directories(/home/nduarte/software/msgpack-c/build/INSTALL/include)

enable_testing()
add_executable(
    sub #demo
    subscriber.cpp
    )

target_link_libraries(
    sub
    cppzmq
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
