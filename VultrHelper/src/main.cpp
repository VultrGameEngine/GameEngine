#include <Vultr.hpp>
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
    std::vector<Token> fileTokens = fileScanner.ScanTokens();
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
        File expected_file = File(generated.GetPath() / f.GetName());
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
            File generated_file = generated.CreateFile(f.GetName());
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

int main(void)
{
    Directory systems(std::filesystem::current_path() / "include/system_providers/");
    GenerateInDir(systems);
    Directory components(std::filesystem::current_path() / "include/components/");
    GenerateInDir(components);
    // std::string path = "include/components/test_component.h";
    // std::string full_path = Path::GetFullPath(path);
    // ScriptScanner fileScanner = ScriptScanner(full_path);
    // std::vector<Token> fileTokens = fileScanner.ScanTokens();
    // ScriptParser fileParser = ScriptParser(fileTokens, full_path);
    // fileParser.Parse();

    // std::string out_path = Path::GetFullPath("res/out/test_file.generated.h");
    // std::ofstream myfile;
    // myfile.open(Path::GetFullPath("res/out/test_file.generated.h"));
    // myfile << fileParser.GenerateHeaderFile().c_str();
    // myfile.close();
    // IFile::WriteFile(fileParser.GenerateHeaderFile().c_str(), path);

    // GenerateInitFiles();
    // Engine &vultr = Engine::Get();

    // float lastTime = 0;
    // vultr.Init(true);
    // vultr.LoadGame("/home/brandon/Dev/GameEngine/SandboxTesting/build/Debug/"
    //                "libSandboxTesting.so");

    // while (!vultr.should_close)
    // {
    //     vultr.UpdateGame(lastTime);
    //     Vultr::Editor::Editor::Get()->Render();
    //     glfwSwapBuffers(vultr.window);
    //     glfwPollEvents();
    // }
    // vultr.Destroy();
}
