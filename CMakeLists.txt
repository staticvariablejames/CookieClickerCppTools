cmake_minimum_required(VERSION 3.10)
project("Cookie Clicker C++ Tools")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(test test.cpp test_random.cpp test_random_utils.cpp test_stock.cpp test_stock_analysis.cpp random.cpp stock_analysis.cpp)
add_executable(generate-database generate_database.cpp random.cpp stock_analysis.cpp)
add_executable(compare-prng compare_prng.cpp random.cpp stock_analysis.cpp)

add_subdirectory(Catch2)
target_link_libraries(test Catch2::Catch2)
