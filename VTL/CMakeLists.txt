cmake_minimum_required(VERSION 3.19.4)
set(This VultrTemplateLibrary)
project(This VERSION 0.0.1)

enable_testing()

file(GLOB_RECURSE Sources
  "tests/*.cpp"
)
set(CMAKE_CXX_STANDARD 17)

include_directories(include )
find_package(GTest CONFIG REQUIRED)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC GTest::gtest)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_DEBUG_MACRO")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftime-trace")
