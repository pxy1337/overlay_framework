generate:
	cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DBUILD_EXAMPLE=On -DBUILD_TESTS=On -S . -B build

compile:
	cmake --build build
	.\build\bin\tests.exe

execute:
	.\build\bin\overlay_example.exe

run: generate compile execute 
