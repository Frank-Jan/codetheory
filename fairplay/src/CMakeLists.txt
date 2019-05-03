cmake_minimum_required(VERSION 3.12)
project(fairplay)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH /home/fj/Documents/2018-2019/semester_2/codetheory/fairplay)
add_executable(fairplay main.cpp readFile.cpp readFile.h exceptions.h utils.cpp utils.h)
