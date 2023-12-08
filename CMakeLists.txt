cmake_minimum_required(VERSION 3.5)

project(data_structures)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR})

add_compile_options(-Wall -Wextra)

add_subdirectory(list)
add_subdirectory(queue)
add_subdirectory(tree)
