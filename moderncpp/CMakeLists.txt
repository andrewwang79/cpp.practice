cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14) # 11

set(TARGET_NAME "moderncpp")
project(${TARGET_NAME})

# moderncpp
if(UNIX)
    add_executable(${TARGET_NAME} main.cpp)
    TARGET_LINK_LIBRARIES(${TARGET_NAME} pthread)
endif()

# DynamicCreate
if(UNIX)
    add_executable(DynamicCreate DynamicCreate.cpp)
endif()

# Executor
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(Executor Executor.cpp)

if(UNIX)
    TARGET_LINK_LIBRARIES(Executor pthread)
endif()