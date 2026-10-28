Run:
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
cmake -S . -B build -G Ninja
cmake --build build
.\build\pbl2_gui.exe

Tai khoan mac dinh: admin / admin123
