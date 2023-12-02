cmake_minimum_required(VERSION 3.5)

project(test_tree)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR} 
${PROJECT_SOURCE_DIR}/..
)

add_subdirectory(b_tree)
add_subdirectory(bs_tree)
add_subdirectory(rb_tree)
add_subdirectory(avl_tree)
