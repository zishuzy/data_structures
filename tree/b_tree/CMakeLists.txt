cmake_minimum_required(VERSION 3.5)

project(b_tree)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR} 
${PROJECT_SOURCE_DIR}/..
${PROJECT_SOURCE_DIR}/../..
)

add_executable(b_tree_test b_tree_test.cpp)
