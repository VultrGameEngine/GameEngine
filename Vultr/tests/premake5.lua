workspace "VultrTests"
    toolset("clang")
    architecture "x64"
    configurations {
      "Debug",
    }


project "Vultr"
   kind "ConsoleApp"
   language "C++"
   targetdir "../build/tests/"

   files 
   { 
      "**.cpp",
      "**.c" 
    }

    includedirs 
    {
      "../include",
      "../vendor"
    }
    libdirs {
      "/home/brandon/Dev/GameEngine/Vultr/build/Debug",
      os.findlib("assimp"),
      os.findlib("freetype"),
      os.findlib("X11"),
      os.findlib("GL"),
      os.findlib("Xxf86vm"),
      os.findlib("Xrandr"),
      os.findlib("pthread"),
      os.findlib("Xi"),
      os.findlib("dl"),
      os.findlib("glfw3"),
    }

    links { "glfw3", "assimp", "Vultr", "X11", "GL", "Xxf86vm", "Xrandr", "pthread", "Xi", "dl", "freetype", "gtest"}

    cppdialect "C++17"

    filter "configurations:Debug"
      defines { "DEBUG", "ENABLE_DEBUG_MACRO" }
      symbols "On"
