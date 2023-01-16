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

set(directory "${PROJECT_SOURCE_DIR}/src/rythe")
file(GLOB_RECURSE sources *)
list(FILTER sources INCLUDE REGEX ${directory})
list(FILTER sources INCLUDE REGEX "(src\\/rythe\\/.*\\.hpp)|(src\\/rythe\\/.*\\.cpp)|(src\\/rythe\\/.*\\.h)|(src\\/rythe\\/.*\\.c)|(src\\/rythe\\/.*\\.inl)|(src\\/rythe\\/[^\\.]*)")
add_library(rythe-standard-library STATIC ${sources})

foreach(src ${sources})
	get_filename_component(SRC_EXT ${src} EXT)
	get_filename_component(dir ${src} DIRECTORY)
	file(RELATIVE_PATH rel_path ${PROJECT_SOURCE_DIR}/src/rythe ${dir})
	source_group("${rel_path}" FILES "${src}")
endforeach()

set_target_properties(rythe-standard-library PROPERTIES LINKER_LANGUAGE ${RYTHE_LANGUAGE})

target_include_directories(rythe-standard-library PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(rythe-standard-library PUBLIC ${RYTHE_DEFINITIONS})
target_compile_features(rythe-standard-library PUBLIC cxx_std_20)

if(NOT DEFINED RYTHE_COMPILER_FLAGS)
	set(RYTHE_COMPILER_FLAGS "")

	if (NOT MSVC)
		set(RYTHE_COMPILER_FLAGS -Wall -Wextra -Wpedantic
								-Wno-c++98-compat -Wno-c++98-compat-pedantic 
								-Wno-c++0x-compat 
								-Wno-c++11-compat -Wno-c++11-compat-pedantic
								-Wno-c++14-compat -Wno-c++14-compat-pedantic
								-Werror
								${RYTHE_COMPILER_FLAGS})
	else()
		set(RYTHE_COMPILER_FLAGS /MP /W4 /WX ${RYTHE_COMPILER_FLAGS})
	endif()
endif()

target_compile_options(rythe-standard-library PUBLIC ${RYTHE_COMPILER_FLAGS})

if(DEFINED RYTHE_LINKER_FLAGS)
	target_link_options(rythe-standard-library PUBLIC ${RYTHE_LINKER_FLAGS})
endif()
