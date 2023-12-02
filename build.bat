@echo off

if not exist bin mkdir bin

pushd bin

@echo on

em++ -o MosaicoSIM.html -O0 -g -Xclang -gcodeview -D_DEBUG -D_DLL -D_MT -Xclang --dependent-lib=msvcrtd -MD -MT -MF ..\src\tfg_main.cpp -sUSE_WEBGL2=1 -sALLOW_MEMORY_GROWTH -sTOTAL_MEMORY=16MB --shell-file=..\src\shell.html && popd || popd

@echo off