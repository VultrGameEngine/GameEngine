cmake_minimum_required(VERSION 3.0.0)
project(VultrHelper VERSION 0.1.0)


file(GLOB_RECURSE VultrEditor_SRC
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
    "include/*.cpp"
    "vendor/*.h"
    "vendor/*.cpp"
)

include_directories(include vendor ../Vultr/include ../Vultr/vendor)

set(CMAKE_CXX_STANDARD 17)
add_executable(VultrHelper ${VultrEditor_SRC})
target_link_libraries(VultrHelper GL glfw3 X11 Xxf86vm Xrandr pthread Xi dl assimp /home/brandon/Dev/GameEngine/Vultr/build/libVultrShared.so)

