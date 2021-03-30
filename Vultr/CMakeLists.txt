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

find_package(ASSIMP REQUIRED)
include_directories(include vendor /usr/include/freetype2)
include_directories(${ASSIMP_INCLUDE_DIRS})


add_library(VultrShared SHARED ${Vultr_SRC})
# add_library(Vultr STATIC ${Vultr_SRC})
target_link_libraries(VultrShared GL glfw3 X11 Xxf86vm Xrandr pthread Xi dl freetype ${ASSIMP_LIBRARIES})
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_DEBUG_MACRO")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
