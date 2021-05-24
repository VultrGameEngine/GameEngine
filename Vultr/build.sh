/usr/bin/cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/bin/clang-11 -DCMAKE_CXX_COMPILER:FILEPATH=/bin/clang++ -H. -Bbuild -G "Unix Makefiles"

/usr/bin/cmake --build build --config Debug --target all -- -j 10
ClangBuildAnalyzer --all build/CMakeFiles/VultrShared.dir build/capture_file
ClangBuildAnalyzer --analyze build/capture_file
