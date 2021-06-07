cmake_minimum_required (VERSION 3.8)

project ("libLoad")

set(CMAKE_INSTALL_PREFIX /usr/)

add_subdirectory(testLibX)
add_subdirectory(compileDemo)
add_subdirectory(sysDemo)
add_subdirectory(dynamicDemo)
