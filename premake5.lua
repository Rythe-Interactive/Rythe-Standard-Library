--! Legion Central Build Script for Legion-Engine
--[[
authors: Raphael Baier, Glyn Leine
copyright: (c) 2020 Raphael Baier, The Legion-Team

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

]]--
function createProject(groupName,projectName,kindName)
    print("Building " .. projectName)
    group ("" .. groupName)
    project ("" .. projectName)
        kind (""..kindName)
        location ("src/"..projectName)
        architecture "x64"
        toolset "clang"
        language "C++"
        cppdialect "C++20"
        targetdir "$(SolutionDir)bin\\lib"
        libdirs {"$(SolutionDir)bin\\lib\\"}
        objdir "$(SolutionDir)bin\\obj"
        defines {"ARBROOK_INTERNAL", "PROJECT_NAME="..projectName}
        filter "configurations:Debug*"
            defines {"DEBUG"}
            symbols "On"
            targetsuffix "-d"
        filter "configurations:Release*"
            defines {"NDEBUG"}
            optimize "On"
        filter "configurations:*OGL"
            defines {"RenderingAPI_OGL=0","RenderingAPI=0"}
        
        filter "configurations:*DX11"
            defines {"RenderingAPI_DX11=1","RenderingAPI=1"}  

        filter {}
    group ""
end

-- root workspace, all sub-project should be included
workspace "rsl"
    configurations { "Debug64", "Release64" }
    
include "build-rythe-standard-library.lua"

project "*"
    includedirs { "src/rsl/impl/"}
    targetdir "build/%{cfg.buildcfg}"
    libdirs { "lib/", "build/%{cfg.buildcfg}/" }
    toolset "clang"

        
    filter "configurations:Debug*"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release*"
        defines {"NDEBUG"}
        optimize "On"

    filter "configurations:*64"
        architecture "x86_64"

