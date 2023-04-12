cmake_minimum_required (VERSION 3.8)

project ("libLoad")

set(CMAKE_INSTALL_PREFIX /usr/)

add_subdirectory(testLibX)
add_subdirectory(compileDemo)
add_subdirectory(dynamicDemo)
# add_subdirectory(sysDemo) # testLibX install后可开启