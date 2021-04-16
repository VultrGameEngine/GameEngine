workspace "VultrEditor"
    toolset("clang")
    architecture "x64"
    configurations {
      "Debug",
      "Release",
      "Dist" 
    }


project "VultrEditor"
   kind "WindowedApp"
   language "C++"
   targetdir "build/%{cfg.buildcfg}"

   files 
   { 
      "include/**.h", 
      "include/**.c",
      "include/**.cpp",
      "include/**.hpp",
      "vendor/**.cpp",
      "src/**.cpp",
      "src/**.c" 
    }

    includedirs 
    {
      "include",
      "vendor",
      "../Vultr/include",
      "../Vultr/vendor"
    }
    linkoptions{ "-R/home/brandon/Dev/GameEngine/Vultr/build/Debug" }

    runpathdirs 
    { 
      "../Vultr/build/Debug",
      "../Vultr/vendor/libs/linux",
      os.findlib("yaml-cpp"),
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
    libdirs 
    {
      "../Vultr/build/Debug",
      os.findlib("yaml-cpp"),
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

    links { "glfw3", "assimp", "Vultr", "X11", "GL", "Xxf86vm", "Xrandr", "pthread", "Xi", "dl", "freetype", "yaml-cpp"}

    cppdialect "C++17"

   filter "configurations:Debug"
      defines { "DEBUG", "ENABLE_DEBUG_MACRO" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

