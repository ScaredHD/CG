cmake_minimum_required(VERSION 3.0.0)
project(helloworld)

set(CMAKE_PREFIX_PATH "D:/GAMES101/glfw/lib/cmake/glfw3;D:/GAMES101/assimp/lib/cmake/assimp-5.2")


add_executable(helloworld 
    main.cpp glad.cpp 
    src/mesh.cpp
    src/model.cpp
)

include_directories("./include")

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(helloworld glfw)

include_directories("D:/GAMES101/glm")

find_package(assimp 5.2.5 REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(helloworld ${ASSIMP_LIBRARIES})