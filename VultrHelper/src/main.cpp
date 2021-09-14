#include <iostream>
#include <scripting/script_parser.h>
#include <scripting/script_scanner.h>
#include <helpers/file_outputter.h>
#include <set>

using namespace Vultr;
void ParseFile(HeaderAndSourceFile &source, HeaderAndSourceFile &output_source, HeaderAndSourceFile &output_header)
{
    std::cout << "Parsing " << source.path << std::endl;
    {
        FileOutputter outputter(output_source);
        ScriptScanner fileScanner = ScriptScanner(source);
        auto fileTokens = fileScanner.ScanTokens();
        ScriptParser fileParser = ScriptParser(fileTokens, source);
        fileParser.Parse();

        outputter << fileParser.GenerateSourceFile().c_str();
    }
    {
        FileOutputter outputter(output_header);
        ScriptScanner fileScanner = ScriptScanner(source);
        auto fileTokens = fileScanner.ScanTokens();
        ScriptParser fileParser = ScriptParser(fileTokens, source);
        fileParser.Parse();

        outputter << fileParser.GenerateHeaderFile().c_str();
    }
}

void GenerateInDir(Directory dir)
{
    Directory generated = Directory(&dir, "generated");
    dirmake(&generated);

    size_t file_count;
    auto _f = dirfiles(&generated, &file_count);

    auto files = std::set(_f.begin(), _f.end());
    std::set<HeaderAndSourceFile> remaining_files;

    for (File f : files)
    {
        remaining_files.insert(f);
    }
    for (File f : directory_get_files(dir))
    {
        std::string name_no_extensions = file_get_name(f);
        name_no_extensions = name_no_extensions.substr(0, name_no_extensions.size() - 2);
        HeaderAndSourceFile expected_source_file = HeaderAndSourceFile((generated.path / (name_no_extensions + ".generated.cpp")).string());
        HeaderAndSourceFile expected_header_file = HeaderAndSourceFile((generated.path / (name_no_extensions + ".generated.h")).string());
        // If we already have a generated file
        if (files.find(expected_source_file) != files.end())
        {
            HeaderAndSourceFile generated_source_file = *files.find(expected_source_file);
            HeaderAndSourceFile generated_header_file = *files.find(expected_header_file);
            if (file_get_date_modified(f) > file_get_date_modified(generated_source_file) || file_get_date_modified(f) > file_get_date_modified(generated_header_file))
            {
                ParseFile(f, generated_source_file, generated_header_file);
            }
        }
        else
        {
            HeaderAndSourceFile generated_source_file = create_file(generated, (name_no_extensions + ".generated.cpp").c_str());
            HeaderAndSourceFile generated_header_file = create_file(generated, (name_no_extensions + ".generated.h").c_str());
            ParseFile(f, generated_source_file, generated_header_file);
        }
        remaining_files.erase(expected_source_file);
        remaining_files.erase(expected_header_file);
    }

    for (HeaderAndSourceFile f : remaining_files)
    {
        std::cout << "Removing generated " << file_get_name(f) << std::endl;
        fremove(&f);
    }

    for (Directory d : directory_get_sub_directories(dir))
    {
        if (d != generated)
        {
            GenerateInDir(d);
        }
    }
}

int main(int argc, char *argv[])
{
    Directory current;
    dircurrentworking(&current);
    dirmake("build", &current);

    Directory systems(&current, "include/system_providers/");
    GenerateInDir(systems);

    Directory components(&current, "include/components/");
    GenerateInDir(components);

#ifndef _WIN32
    system("/usr/bin/cmake --no-warn-unused-cli "
           "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug "
           "-DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang-12 "
           "-DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ "
           "build -G \"Unix Makefiles\"");
    system("/usr/bin/cmake --build build "
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
#endif
}
