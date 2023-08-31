createProject ("include","rsl","StaticLib")
includedirs {
    "$(SolutionDir)include/rythe-standard-library/src/",
    "src/"
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

