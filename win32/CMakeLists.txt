cmake_minimum_required(VERSION 3.20)

project(win32_test)

add_executable(win32_test test.cpp window.cpp)

set (CMAKE_CXX_FLAGS "-municode")
