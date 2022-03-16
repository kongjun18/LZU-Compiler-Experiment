cmake_minimum_required(VERSION 3.18)
project(SimpleParser CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS 0)

# TODO: download and configure fmtlib
set(sources 
    parser.cc
)
set(headers
    parser.h
)
add_executable(SimpleParser main.cc ${sources} ${headers})
