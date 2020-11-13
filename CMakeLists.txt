cmake_minimum_required(VERSION 3.10)
project("Cookie Clicker C++ Tools")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test test.cpp)

add_subdirectory(Catch2)
target_link_libraries(test Catch2::Catch2)
