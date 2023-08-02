createProject ("include","rsl","StaticLib")
includedirs {
    "$(SolutionDir)include/rsl/src/"
}

files { 
    "src/**.h",
    "src/**.hpp",
    "src/**.inl",
    "src/**.c",
    "src/**.cpp"
}

targetdir "build/%{cfg.buildcfg}"
libdirs { 
    "lib/", 
    "build/%{cfg.buildcfg}/"
 }


