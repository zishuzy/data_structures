cmake_minimum_required(VERSION 3.5)

project(binary_tree_c)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/../..
    ${PROJECT_SOURCE_DIR}/../../..
)

add_executable(
    binary_tree_c

    binary_tree_test.c
    binary_tree.c

    ${PROJECT_SOURCE_DIR}/../../../queue/c/queue.c
    ${PROJECT_SOURCE_DIR}/../../../common/utils/utils_string.c
)