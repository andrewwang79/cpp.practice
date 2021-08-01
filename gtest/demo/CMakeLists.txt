cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "demo")
project(${TARGET_NAME})

# SO
file(GLOB SO_SOURCE_FILES *.cc *.cpp *.cxx)
aux_source_directory(src SO_SOURCE_FILES)
add_library(${TARGET_NAME}_lib SHARED ${SO_SOURCE_FILES} )

# UT
IF(ENABLE_UT)
  # GoogleTest requires at least C++11
  set(CMAKE_CXX_STANDARD 11)
  file(GLOB SOURCE_FILES *.cc *.cpp *.cxx)
  aux_source_directory(src SOURCE_FILES)
  aux_source_directory(test SOURCE_FILES)

  add_executable(
    ${TARGET_NAME}
    ${SOURCE_FILES}
  )
  target_link_libraries(
    ${TARGET_NAME}
    gmock_main
  )

  include(GoogleTest)
  gtest_discover_tests(${TARGET_NAME})
ENDIF()
