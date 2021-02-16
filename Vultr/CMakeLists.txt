cmake_minimum_required(VERSION 3.0.0)
project(Vultr VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE Vultr_SRC
    "src/*.cpp"
    "src/*.c"
    "include/*.h"
    "include/*.hpp"
    "include/*.cpp"
    "vendor/*.h"
    "vendor/*.cpp"
)

include_directories(include vendor)


# add_executable(Vultr ${Vultr_SRC})
add_library(Vultr SHARED ${Vultr_SRC})
target_link_libraries(Vultr GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl GLEW)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
