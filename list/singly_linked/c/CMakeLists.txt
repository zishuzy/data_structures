cmake_minimum_required(VERSION 3.5)

project(slinked_list_c)

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

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "current platform: Linux ")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "current platform: Windows")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "current platform: Mac os x")
    SET(CMAKE_C_COMPILER /opt/homebrew/opt/llvm/bin/clang)
    SET(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)
    SET(INC_PATH_EXTRA "/opt/homebrew/opt/llvm/include" "/opt/homebrew/Cellar/googletest/1.14.0/include")
    SET(LINK_PATH_EXTRA "/opt/homebrew/opt/llvm/lib" "/opt/homebrew/Cellar/googletest/1.14.0/lib")
ELSE()
    message(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

include_directories(
    ${INC_PATH_EXTRA}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/../..
    ${PROJECT_SOURCE_DIR}/../../..
)

link_directories(
    ${LINK_PATH_EXTRA}
)

add_executable(slinked_list_c slinked_list_demo.c slinked_list.c)
add_executable(slinked_list_c_gtest slinked_list_gtest.cpp slinked_list.c)
target_link_libraries(slinked_list_c_gtest gtest gmock)
