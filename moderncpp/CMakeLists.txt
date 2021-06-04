cmake_minimum_required(VERSION 3.1.0)

set(TARGET_NAME "moderncpp")
project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCE_FILES *.cc *.cpp *.cxx)
aux_source_directory(. SOURCE_FILES)

add_executable(${TARGET_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${TARGET_NAME} pthread)
