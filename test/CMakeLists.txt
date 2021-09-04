

set(projName uTest)
project(${projName})

set(CMAKE_CXX_STANDARD 17)

add_executable(${projName} core/UtilityTests.cpp core/Utils.hpp)


