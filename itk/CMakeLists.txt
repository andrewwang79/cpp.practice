cmake_minimum_required(VERSION 3.1.0)

set(TARGET_NAME "itk")
project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB SOURCE_FILES *.cc *.cpp *.cxx)
aux_source_directory(src SOURCE_FILES)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

conan_target_link_libraries(${TARGET_NAME})
