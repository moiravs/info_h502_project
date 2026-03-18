# info_h502_project

compile:
rm -rf build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE=Debug -S . -B build -G Ninja &&  cmake --build build

run:
./build/VRproject 