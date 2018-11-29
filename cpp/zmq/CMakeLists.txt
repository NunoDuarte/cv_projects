cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(cppzmq-demo CXX)

find_package(cppzmq)

enable_testing()
add_executable(
    sub #demo
    subscriber.cpp
    )

target_link_libraries(
    sub
    cppzmq
    )

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
