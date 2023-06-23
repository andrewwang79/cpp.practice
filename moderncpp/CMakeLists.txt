cmake_minimum_required(VERSION 3.1.0)

set(TARGET_NAME "moderncpp")
project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 14)

add_executable(${TARGET_NAME} main.cpp)
if(UNIX)
    TARGET_LINK_LIBRARIES(${TARGET_NAME} pthread)
endif()

add_executable(DynamicCreate DynamicCreate.cpp)

add_executable(Executor Executor.cpp)
if(UNIX)
    TARGET_LINK_LIBRARIES(Executor pthread)
endif()