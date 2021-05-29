cmake_minimum_required(VERSION 3.0.0)
project(VultrEditor VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE VultrEditor_SRC
    "src/*.cpp"
    "include/*.cpp"
    "vendor/*.cpp"
)

include_directories(include vendor res ../Vultr/include ../Vultr/vendor ../VTL/include)

if(UNIX)
    set(CMAKE_CXX_STANDARD 17)
endif(UNIX)

if(WIN32)
    if (MSVC_VERSION GREATER_EQUAL "1900")
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
	if (_cpp_latest_flag_supported)
	    add_compile_options("/std:c++latest")
	endif()
    endif()
endif(WIN32)
add_executable(VultrEditor ${VultrEditor_SRC})
if(UNIX)
    target_link_libraries(VultrEditor GL glfw3 X11 Xxf86vm Xrandr pthread Xi dl assimp)
    target_link_libraries(VultrEditor ${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/build/libVultrShared.so)
    target_compile_definitions(VultrEditor PRIVATE SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")
endif(UNIX)

if(WIN32)
    add_executable(Incbin incbin/incbin.c)
    find_library(GLFW3 glfw3 HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/vendor/libs/WIN32)
    find_library(FREETYPE freetyped HINTS  ${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/vendor/libs/WIN32)
    find_library(ASSIMP assimp HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/vendor/libs/WIN32)
    find_library(VULTR Vultr HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/out/build/x64-Debug/Vultr.lib)
    target_link_libraries(VultrEditor ${GLFW3} ${FREETYPE} ${ASSIMP} opengl32 ${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/out/build/x64-Debug/Vultr.lib)
    add_custom_target(incbin_run
        COMMAND Incbin editor/editor.cpp main.cpp -o data.cpp -Ssnakecase 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "run incbin in ${CMAKE_CURRENT_SOURCE_DIR}/src"
        SOURCES src/data.cpp
    )
    add_dependencies(VultrEditor incbin_run)
    add_custom_command(TARGET VultrEditor POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different  
	    "${CMAKE_CURRENT_SOURCE_DIR}/../Vultr/vendor/libs/WIN32/assimp-vc142-mtd.dll"
	    $<TARGET_FILE_DIR:VultrEditor>)

endif(WIN32)
