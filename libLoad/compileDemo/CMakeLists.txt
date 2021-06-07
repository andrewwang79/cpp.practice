cmake_minimum_required (VERSION 3.8)

set(TARGET_NAME "compileDemo")
project (${TARGET_NAME})

FILE(GLOB SOURCE_FILES "*.cpp")

add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} testLibX)
