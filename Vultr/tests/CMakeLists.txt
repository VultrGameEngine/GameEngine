cmake_minimum_required(VERSION 3.20.0)

set(This VultrTests)
file(GLOB_RECURSE Sources
    "*.cpp"
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC VultrShared)

add_test(
  NAME ${This}
  COMMAND ${This}
)
