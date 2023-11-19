cmake_minimum_required(VERSION 3.21)
project(MosaicoSIM)

add_executable(MosaicoSIM src/tfg_main.cpp)

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    target_link_options(MosaicoSIM PUBLIC -sUSE_WEBGL2=1 --shell-file=../shell.html)
endif()