cmake_minimum_required(VERSION 3.20)

project(rt2)

add_executable(rt2
    main.cpp
)

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -no-pie")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")