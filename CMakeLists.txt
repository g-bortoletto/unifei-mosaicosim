cmake_minimum_required(VERSION 3.21)
project(MosaicoSIM)
set(CMAKE_BUILD_TYPE Debug)

add_executable(MosaicoSIM src/tfg_main.cpp)

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    target_link_options(MosaicoSIM PUBLIC -sUSE_WEBGL2=1 --shell-file=../src/shell.html)
endif()