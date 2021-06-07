cmake_minimum_required (VERSION 3.8)

set(TARGET_NAME "dynamicDemo")
project (${TARGET_NAME})

FILE(GLOB SOURCE_FILES "*.cpp")

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS}) # 动态加载so的库libdl.so，相当于gcc -ldl
