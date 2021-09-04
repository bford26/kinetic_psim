cmake_minimum_required(VERSION 3.16)

# PROJECT DETAILS
set(projName KPsim)
project(${projName})

set(CMAKE_CXX_STANDARD 17)

# add Cmakelist.txt from roo/lib folder
add_subdirectory(lib)

# create file to run sim
add_executable(${projName} lib/src/es_pic/main.cpp)#${ES_SRC_FILES})

target_link_libraries(${projName} PRIVATE core)