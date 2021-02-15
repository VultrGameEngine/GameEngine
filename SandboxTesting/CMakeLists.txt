cmake_minimum_required(VERSION 3.0.0)
project(Game VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE Game_SRC
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
    "include/*.cpp"
    "vendor/*.h"
    "vendor/*.cpp"
)

include_directories(include vendor ../Vultr/include ../Vultr/vendor)

add_library(Game SHARED ${Game_SRC})
target_link_libraries(Game GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl GLEW /home/brandon/Dev/GameEngine/Vultr/build/libVultr.so)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
