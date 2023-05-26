cmake_minimum_required(VERSION 3.20)

project(rasterizer_v1)

set(CppFiles
    src/screen/window.cpp
    src/camera/camera.cpp
    src/math/mathutils.cpp
    src/math/transformation.cpp
    src/math/vec.cpp
    src/math/matrix.cpp
    src/rasterizer/rasterizer.cpp
    src/rasterizer/shader.cpp
    src/geometry/geometry.cpp
    src/geometry/testGeometry.cpp
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