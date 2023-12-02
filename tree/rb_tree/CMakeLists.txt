cmake_minimum_required(VERSION 3.5)

project(rb_tree)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/../..
)

add_library(rb_tree_c
    OBJECT
    rb_tree_c.c
)

add_library(rb_tree
    OBJECT
    rb_tree.c
)

add_executable(test_rbtree_cpp test_rb_cpp.cpp)
add_executable(test_rbtree_c test_rb.c)

target_link_libraries(test_rbtree_c
    PRIVATE
    rb_tree
)

