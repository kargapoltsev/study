cmake_minimum_required(VERSION 3.18)
project(Study)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

add_subdirectory(Meyers_11_14)
add_subdirectory(Williams_Concurrency_in_action)
add_subdirectory(Asynchrony_Back_to_the_future)
