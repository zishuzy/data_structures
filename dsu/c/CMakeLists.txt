cmake_minimum_required(VERSION 3.5)

project(dsu_c)

if(NOT DEFINED CMAKE_C_STANDARD)
    message("Set CMAKE_C_STANDARD as 11")
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    message("Set CMAKE_CXX_STANDARD as 20")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    message(STATUS "CMAKE_BUILD_TYPE is not set. Defaulting to Debug.")
endif()

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "current platform: Linux ")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "current platform: Windows")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "current platform: Mac os x")
    find_program(LLVM_CLANG clang HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin)
    find_program(LLVM_CLANGPP clang++ HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin)

    if(LLVM_CLANG AND LLVM_CLANGPP)
        SET(CMAKE_C_COMPILER ${LLVM_CLANG})
        SET(CMAKE_CXX_COMPILER ${LLVM_CLANGPP})
    else()
        message(WARNING "LLVM compiler not found, using default compiler")
    endif()
ELSE()
    message(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

find_package(GTest REQUIRED)

include_directories(
    ${INC_PATH_EXTRA}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/../..
)

link_directories(
    ${LINK_PATH_EXTRA}
)

add_executable(dsu_demo dsu_demo.c)
target_compile_definitions(dsu_demo PRIVATE __TEST__)
