@echo off

set PROJECT_DIR=C:\Users\guilh\Documents\Dev\unifei\unifei-tfg\
set SOURCE_DIR=%PROJECT_DIR%src
set LIB_DIR=%PROJECT_DIR%lib
set COMPILER_EXE="C:\Program Files\LLVM\bin\clang++.exe"
set COMPILER_FLAGS=-g -std=c++20
set LINKER_FLAGS= -nodefaultlibs -fuse-ld=lld -fms-runtime-lib=dll_dbg -lole32
set BASE_FILES=%SOURCE_DIR%\*.cpp

if not exist bin mkdir bin

pushd bin
cls
clang++ %BASE_FILES% %COMPILER_FLAGS% %LINKER_FLAGS% -o program.exe
popd
