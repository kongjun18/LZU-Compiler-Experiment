cmake_minimum_required(VERSION 3.18)
project(SimpleCompiler CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS 0)

# TODO: download and configure fmtlib
set(
  sources
  lexer.cc
  parser.cc
)
set(
  headers
  lexer.h
  parser.h
)

find_package(fmt REQUIRED)
if(NOT fmt_FOUND)
  message(FATAL_ERROR "fmt is not found, terminate compilation")
endif()

add_executable(SimpleCompiler main.cc ${sources} ${headers})
target_link_libraries(SimpleCompiler fmt::fmt)
