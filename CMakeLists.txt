cmake_minimum_required(VERSION 3.0.0)
project(GameEngine VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE GameEngine_SRC
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
    "include/*.cpp"
    "vendor/stb_image/*.h"
    "vendor/stb_image/*.cpp"
)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(GameEngine ${GameEngine_SRC})
target_link_libraries(GameEngine GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl GLEW gtest gtest_main)
FILE(COPY res/shaders/Basic.shader DESTINATION "${CMAKE_BINARY_DIR}/res/shaders")
FILE(COPY res/shaders/material.glsl DESTINATION "${CMAKE_BINARY_DIR}/res/shaders")
FILE(COPY res/shaders/skybox.glsl DESTINATION "${CMAKE_BINARY_DIR}/res/shaders")
FILE(COPY res/textures/XWing.png DESTINATION "${CMAKE_BINARY_DIR}/res/textures")
FILE(COPY res/textures/skybox/top.jpg DESTINATION "${CMAKE_BINARY_DIR}/res/textures/skybox")
FILE(COPY res/textures/skybox/bottom.jpg DESTINATION "${CMAKE_BINARY_DIR}/res/textures/skybox")
FILE(COPY res/textures/skybox/left.jpg DESTINATION "${CMAKE_BINARY_DIR}/res/textures/skybox")
FILE(COPY res/textures/skybox/right.jpg DESTINATION "${CMAKE_BINARY_DIR}/res/textures/skybox")
FILE(COPY res/textures/skybox/front.jpg DESTINATION "${CMAKE_BINARY_DIR}/res/textures/skybox")
FILE(COPY res/textures/skybox/back.jpg DESTINATION "${CMAKE_BINARY_DIR}/res/textures/skybox")
FILE(COPY res/models/cube.obj DESTINATION "${CMAKE_BINARY_DIR}/res/models")
FILE(COPY res/models/XWing.obj DESTINATION "${CMAKE_BINARY_DIR}/res/models")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
