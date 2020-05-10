workspace "Sorting"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    platforms
    {
        "Win64"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    TargetDir = "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 
    OutputDir = "%{wks.location}/bin/tmp/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "Sorting"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        
        objdir (OutputDir .. "/%{prj.name}")
        targetdir (TargetDir)

        vpaths {
            ["Includes/*"] = "include/**.h",
            ["Sources/*"] = "src/**.cpp"
        }

        includedirs {"include"}

        files {
            "include/**.h",
            "src/**.cpp"
        }

    filter {"configurations:Debug"}
        defines 
        {
            "DEBUG"
        }
        symbols "On"
        runtime "Debug"

    filter {"configurations:Release"}
        defines 
        {
            "NDEBUG"
            , "RELEASE"
        }
        optimize "On"
        runtime "Release"

    filter {"configurations:Release"}
        defines 
        {
            "NDEBUG"
            , "DIST"
        }
        optimize "On"
        runtime "Release"
        targetdir "bin"

    filter {"platforms:Win64"}
        systemversion "latest"