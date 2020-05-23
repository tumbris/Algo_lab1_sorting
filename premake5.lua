function string:split(sep)
   local sep, fields = sep or ":", {}
   local pattern = string.format("([^%s]+)", sep)
   self:gsub(pattern, function(c) fields[#fields+1] = c end)
   return fields
end

workspace "Sorting"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
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
        debugargs { "-o output.txt", "-N 10000000"}

    filter {"configurations:Release"}
        defines 
        {
            "NDEBUG"
            , "RELEASE"
        }
        optimize "On"
        runtime "Release"
        targetdir "bin"

    filter {"platforms:Win64"}
        systemversion "latest"