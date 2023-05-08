@echo off

set PROJECT_DIR=C:\Users\guilh\Documents\Dev\unifei\unifei-tfg\
set SOURCE_DIR=%PROJECT_DIR%src
set LIB_DIR=%PROJECT_DIR%lib
set COMPILER_EXE="C:\Program Files\LLVM\bin\clang++.exe"
set COMPILER_FLAGS=-g -std=c++20
set LINKER_FLAGS= -nodefaultlibs -fuse-ld=lld -fms-runtime-lib=static -lole32
set BASE_FILES=%SOURCE_DIR%\*.cpp

cd %PROJECT_DIR%

if exist bin rmdir /S /Q bin

mkdir bin

pushd bin
cls
if "%1"=="-r" (
	clang++ %BASE_FILES% -O3 -std=c++20 %LINKER_FLAGS% -o tfg.exe) else (
	clang++ -D DEBUG %BASE_FILES% %COMPILER_FLAGS% %LINKER_FLAGS%  -o tfg.exe)
popd

echo 
