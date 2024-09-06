cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 17)

set(TARGET_NAME "moderncpp")
project(${TARGET_NAME})

if(WIN32)
    add_compile_options(/utf-8)
endif()

# moderncpp
if(UNIX)
    add_executable(${TARGET_NAME} main.cpp)
    TARGET_LINK_LIBRARIES(${TARGET_NAME} pthread)
endif()

# DynamicCreate
if(UNIX)
    add_executable(DynamicCreate DynamicCreate.cpp)
endif()

# ExecPoolTest
add_subdirectory(ExecPool)

# ThreadTest
add_executable(ThreadTest ThreadTest.cpp)
if(UNIX)
    TARGET_LINK_LIBRARIES(ThreadTest pthread)
endif()