cmake_minimum_required (VERSION 3.8)

set(TARGET_NAME "testLibX")
project (${TARGET_NAME})

FILE(GLOB SOURCE_FILES "*.c")
add_library(${TARGET_NAME} SHARED ${SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
FILE(GLOB HEAD_FILES "*.h")
install (FILES ${HEAD_FILES} DESTINATION include/testLibX)
