cmake_minimum_required(VERSION 3.0.0)
project(VultrEditor VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE VultrEditor_SRC
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
    "include/*.cpp"
    "vendor/*.h"
    "vendor/*.cpp"
)

include_directories(include vendor ../Vultr/include ../Vultr/vendor)

add_executable(VultrEditor ${VultrEditor_SRC})
target_link_libraries(VultrEditor GL glfw3 X11 Xxf86vm Xrandr pthread Xi dl assimp /home/brandon/Dev/GameEngine/Vultr/build/libVultr.a /home/brandon/Dev/GameEngine/Vultr/vendor/RmlUi/Build/libRmlCore.a freetype)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
