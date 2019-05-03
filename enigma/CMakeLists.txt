cmake_minimum_required(VERSION 3.13)
project(Groepswerk)

set(CMAKE_CXX_STANDARD 17)

add_executable(enigma main.cpp Enigma.cpp Enigma.h Rotor.cpp Rotor.h)
