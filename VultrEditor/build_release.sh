/usr/bin/cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_C_COMPILER:FILEPATH=/bin/clang-11 -DCMAKE_CXX_COMPILER:FILEPATH=/bin/clang++ -H/home/brandon/Dev/GameEngine/VultrEditor -B/home/brandon/Dev/GameEngine/VultrEditor/release -G "Unix Makefiles"

/usr/bin/cmake --build /home/brandon/Dev/GameEngine/VultrEditor/release --config Release --target all -- -j 10
