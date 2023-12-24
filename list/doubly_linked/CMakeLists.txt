cmake_minimum_required(VERSION 3.5)

project(dlinked_list)

if(NOT DEFINED CMAKE_C_STANDARD)
    message("Set CMAKE_C_STANDARD as 11")
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    message("Set CMAKE_C_STANDARD as 20")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    message("Set CMAKE_BUILD_TYPE as Debug")
endif()

include_directories(${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/../..
)

add_subdirectory(c)
