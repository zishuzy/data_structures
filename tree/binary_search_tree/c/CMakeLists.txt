cmake_minimum_required(VERSION 3.5)

project(binary_search_tree_c)

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
    bs_tree_c

    bs_tree_test.c
    bs_tree.c

    ${PROJECT_SOURCE_DIR}/../../../queue/c/queue.c
    ${PROJECT_SOURCE_DIR}/../../../common/utils/utils_string.c
)