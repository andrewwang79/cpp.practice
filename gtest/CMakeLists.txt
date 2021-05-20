cmake_minimum_required(VERSION 3.14)
project(unittestDemo)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  unittestDemo
  hello_test.cc
)
target_link_libraries(
  unittestDemo
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(unittestDemo)


# coverage option
OPTION (ENABLE_COVERAGE "Use gcov" ON)
MESSAGE(STATUS ENABLE_COVERAGE=${ENABLE_COVERAGE})
IF(ENABLE_COVERAGE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF()
