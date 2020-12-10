cmake_minimum_required(VERSION 3.0.0)
project(GameEngine VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE GameEngine_SRC
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
    "include/*.cpp"
)

add_executable(GameEngine ${GameEngine_SRC})
target_link_libraries(GameEngine GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl GLEW)
FILE(COPY res/shaders/Basic.shader DESTINATION "${CMAKE_BINARY_DIR}/res/shaders")
FILE(COPY res/models/cube.obj DESTINATION "${CMAKE_BINARY_DIR}/res/models")
FILE(COPY res/models/XWing.obj DESTINATION "${CMAKE_BINARY_DIR}/res/models")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
