cmake_minimum_required(VERSION 3.0.0)
project(VultrHelper VERSION 0.1.0)


file(GLOB_RECURSE VultrEditor_SRC
    "src/*.cpp"
    "include/*.cpp"
)

include_directories(include vendor ../Vultr/include ../Vultr/vendor ../VTL/include)

set(CMAKE_CXX_STANDARD 17)
add_executable(VultrHelper ${VultrEditor_SRC})
if(WIN32)
target_link_libraries(VultrHelper ${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/out/build/x64-Debug/Vultr.lib)
endif(WIN32)
if(UNIX)
target_link_libraries(VultrHelper GL glfw3 X11 Xxf86vm Xrandr pthread Xi dl assimp ${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/build/libVultrShared.so)
endif(UNIX)

