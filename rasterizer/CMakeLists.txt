cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG   "-O2" )

find_package(OpenCV REQUIRED)
include_directories("D:/GAMES101/eigen")

add_executable (
    Rasterizer
    main.cpp
    rasterizer.h rasterizer.cpp
    math.h math.cpp
    geometry.h geometry.cpp
    transformation.h transformation.cpp
    camera.h camera.cpp
)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
