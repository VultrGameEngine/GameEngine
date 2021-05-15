// #include <vultr.hpp>
#include <iostream>
#include <scripting/script_parser.h>
#include <scripting/script_scanner.h>
#include <helpers/path.h>
#include <helpers/file_outputter.h>

using namespace Vultr;
void ParseFile(File &source, File &output)
{
    std::cout << "Parsing " << source.GetName() << std::endl;
    FileOutputter outputter(output);
    ScriptScanner fileScanner = ScriptScanner(source);
    auto fileTokens = fileScanner.ScanTokens();
    ScriptParser fileParser = ScriptParser(fileTokens, source);
    fileParser.Parse();

    outputter << fileParser.GenerateHeaderFile().c_str();
}

void GenerateInDir(Directory dir)
{
    Directory generated = dir.CreateSubDirectory("generated");
    std::set<File> files = generated.GetFiles();
    std::set<File> remaining_files;
    for (File f : files)
    {
        remaining_files.insert(f);
    }
    for (File f : dir.GetFiles())
    {
        File expected_file = File(generated.GetPath() / (f.GetName() + ".cpp"));
        // If we already have a generated file
        if (files.find(expected_file) != files.end())
        {
            File generated_file = *files.find(expected_file);
            if (f.GetDateModified() > generated_file.GetDateModified())
            {
                ParseFile(f, generated_file);
            }
        }
        else
        {
            File generated_file = generated.CreateFile(f.GetName() + ".cpp");
            ParseFile(f, generated_file);
        }
        remaining_files.erase(expected_file);
    }

    for (File f : remaining_files)
    {
        std::cout << "Removing generated " << f.GetName() << std::endl;
        f.Delete();
    }
    for (Directory d : dir.GetDirectories())
    {
        if (d != generated)
        {
            GenerateInDir(d);
        }
    }
}

int main(int argc, char *argv[])
{
    Directory current(std::filesystem::current_path());
    current.CreateSubDirectory("build");
    Directory systems(std::filesystem::current_path() / "include/system_providers/");
    GenerateInDir(systems);
    Directory components(std::filesystem::current_path() / "include/components/");
    GenerateInDir(components);
    system("/usr/bin/cmake --no-warn-unused-cli "
           "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug "
           "-DCMAKE_C_COMPILER:FILEPATH=/bin/clang-11 "
           "-DCMAKE_CXX_COMPILER:FILEPATH=/bin/clang++ "
           "build -G \"Unix Makefiles\"");
    system("/usr/bin/cmake --build /home/brandon/Dev/GameEngine/SandboxTesting/build "
           "--config Debug --target all -- -j 10");
    if (argc >= 3 && std::string(argv[1]) == "--g")
    {
        std::string game = argv[2];
        for (int i = 3; i < argc; i++)
        {
            std::string param = std::string(argv[i]);
            if (param == "-d")
            {
                std::string main_call = "ClangBuildAnalyzer --all build/CMakeFiles/" + game + ".dir build/capture_file";
                std::cout << main_call << std::endl;
                system(main_call.c_str());
                system("ClangBuildAnalyzer --analyze build/capture_file");
            }
        }
    }
}
