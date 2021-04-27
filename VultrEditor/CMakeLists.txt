cmake_minimum_required(VERSION 3.0.0)
project(VultrEditor VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE VultrEditor_SRC
    "src/*.cpp"
    "include/*.cpp"
)

include_directories(include vendor ../Vultr/include ../Vultr/vendor)

set(CMAKE_CXX_STANDARD 17)
add_executable(VultrEditor ${VultrEditor_SRC})
target_link_libraries(VultrEditor GL glfw3 X11 Xxf86vm Xrandr pthread Xi dl assimp /home/brandon/Dev/GameEngine/Vultr/build/libVultrShared.so)
