/usr/bin/cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/bin/clang-11 -DCMAKE_CXX_COMPILER:FILEPATH=/bin/clang++ -H/home/brandon/Desktop/Dev/GameEngine -B/home/brandon/Desktop/Dev/GameEngine/build -G "Unix Makefiles"

/usr/bin/cmake --build /home/brandon/Desktop/Dev/GameEngine/build --config Debug --target all -- -j 10
