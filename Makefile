generate:
	cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DBUILD_EXAMPLE=On -S . -B build

compile:
	cmake --build build

execute:
	.\build\bin\overlay_example.exe

run: generate compile execute 
