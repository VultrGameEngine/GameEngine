workspace "Vultr"
    toolset("clang")
    architecture "x64"
    configurations {
      "Debug",
      "Release",
      "Dist" 
    }


project "Vultr"
   kind "SharedLib"
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
      "vendor"
    }
    libdirs {
      -- os.findlib("assimp"),
      -- os.findlib("freetype"),
      os.findlib("glfw3"),
    }
    links {"glfw3"}

    cppdialect "C++17"

    filter "configurations:Debug"
      defines { "DEBUG", "ENABLE_DEBUG_MACRO" }
      symbols "On"

    filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "On"
