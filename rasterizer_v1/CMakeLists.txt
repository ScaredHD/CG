cmake_minimum_required(VERSION 3.20)

project(rasterizer_v1)

set(CppFiles
    src/screen/buffer.cpp
    src/screen/window.cpp
    src/screen/image.cpp
)

add_executable(rasterizer_v1 
    src/main.cpp
    ${CppFiles}
)

include_directories(
    src/camera
    src/geometry
    src/math
    src/rasterizer
    src/screen
)

set(CMAKE_CXX_FLAGS "-municode")