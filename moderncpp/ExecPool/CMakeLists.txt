set(TARGET_NAME "ExecPoolTest")
project(${TARGET_NAME})

# ExecPoolTest
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${TARGET_NAME} ExecPoolTest.cpp)

if(UNIX)
    TARGET_LINK_LIBRARIES(${TARGET_NAME} pthread)
endif()