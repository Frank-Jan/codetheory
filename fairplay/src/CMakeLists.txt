cmake_minimum_required(VERSION 3.12)
project(fairplay)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ..)
add_executable(fairplay main.cpp readFile.cpp readFile.h exceptions.h utils.cpp utils.h)
