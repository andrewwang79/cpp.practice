cmake_minimum_required (VERSION 3.8)

set(TARGET_NAME "dynamicDemo")
project (${TARGET_NAME})

FILE(GLOB SOURCE_FILES "*.cpp")

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS}) # CMAKE_DL_LIBS值是dl，相当于动态加载动态库libdl.so
