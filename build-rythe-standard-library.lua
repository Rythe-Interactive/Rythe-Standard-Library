createProject ("include","rythe-standard-library","StaticLib")
includedirs {
    "$(SolutionDir)include/rythe-standard-library/src/"
}

files { 
    "src/**.h",
    "src/**.hpp",
    "src/**.inl",
    "src/**.c",
    "src/**.cpp"
}

targetdir "$(SolutionDir)bin\\lib\\"
libdirs { 
    "$(SolutionDir)bin\\lib\\"
 }


