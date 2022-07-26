cmake_minimum_required(VERSION 3.1.0)

set(TARGET_NAME "moderncpp")
project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 14)

add_executable(${TARGET_NAME} main.cpp)
TARGET_LINK_LIBRARIES(${TARGET_NAME} pthread)

add_executable(DynamicCreate DynamicCreate.cpp)