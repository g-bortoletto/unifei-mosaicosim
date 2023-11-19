emcc -std=c++20 -Iinclude ^
    include\imgui\imgui.cpp ^
    include\imgui\imgui_demo.cpp ^
    include\imgui\imgui_draw.cpp ^
    include\imgui\imgui_tables.cpp ^
    include\imgui\imgui_widgets.cpp ^
    include\parson\parson.cpp ^
    src\tfgBackgroundImage.cpp ^
    src\tfgControlBar.cpp ^
    src\tfgDebugInfo.cpp ^
    src\tfgMain.cpp ^
    src\tfgMenuBar.cpp ^
    src\tfgMouse.cpp ^
    src\tfgShape.cpp ^
    src\tfgProgram.cpp ^
    src\tfgRect.cpp ^
    src\tfgVector.cpp ^
    -s WASM=1 ^
    -o MosaicoSIM.html
