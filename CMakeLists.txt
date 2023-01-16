cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Folder structure
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Project
if(NOT DEFINED RYTHE_LIBRARY_TYPE)
	set(RYTHE_LIBRARY_TYPE STATIC)
endif()

if(NOT DEFINED RYTHE_FILE_TYPES)
	set(RYTHE_FILE_TYPES *.h *.hpp *.c *.cpp *.inl)
endif()

if(NOT DEFINED RYTHE_LANGUAGE)
	set(RYTHE_LANGUAGE CXX)
endif()

project(rythe-standard-library LANGUAGES ${RYTHE_LANGUAGE})

set(directory "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE sources *)
list(FILTER sources INCLUDE REGEX ${directory})
list(FILTER sources INCLUDE REGEX "(src\\/.*\\.hpp)|(src\\/.*\\.cpp)|(src\\/.*\\.h)|(src\\/.*\\.c)|(src\\/.*\\.inl)|(src\\/[^\\.]*)")
add_library(rythe-standard-library STATIC ${sources})

foreach(src ${sources})
	get_filename_component(SRC_EXT ${src} EXT)
	get_filename_component(dir ${src} DIRECTORY)
	file(RELATIVE_PATH rel_path ${PROJECT_SOURCE_DIR}/src ${dir})
	source_group("${rel_path}" FILES "${src}")
endforeach()


target_include_directories(rythe-standard-library PUBLIC ${PROJECT_SOURCE_DIR}/src)