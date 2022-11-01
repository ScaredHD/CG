cmake_minimum_required(VERSION 3.0.0)
project(helloworld)

set(CMAKE_PREFIX_PATH "D:/GAMES101/glfw/lib/cmake/glfw3")


add_executable(helloworld main.cpp glad.cpp)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(helloworld glfw)

include_directories("D:/GAMES101/glm")

