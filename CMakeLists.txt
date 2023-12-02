cmake_minimum_required(VERSION 3.5)

project(test_data_structures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR})

add_compile_options(-Wall -Wextra)

add_subdirectory(tree)
add_subdirectory(queue)
