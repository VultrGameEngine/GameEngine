#include <helpers/directory.h>
#include <filesystem>

namespace Vultr
{

namespace fs = std::filesystem;
Directory::Directory(const std::string &p_path) : path(p_path)
{
    for (auto &file : fs::directory_iterator(path))
    {
        if (file.is_directory())
        {
            sub_directories.insert(Directory(file.path().string()));
        }
        else
        {
            files.insert(File(file.path().string()));
        }
    }
}
fs::path Directory::GetPath() const
{
    return path;
}

Directory Directory::CreateSubDirectory(const std::string &name)
{
    for (auto &dir : GetDirectories())
    {
        if (dir.GetName() == name)
        {
            return dir;
        }
    }
    fs::path new_path = GetPath() / name;
    fs::create_directory(new_path);
    return Directory(new_path.string());
}
File Directory::CreateFile(const std::string &name)
{
    for (auto &file : GetFiles())
    {
        if (file.GetName() == name)
        {
            return file;
        }
    }
    std::string new_path = (GetPath() / name).string();
    return File(new_path);
}

void Directory::Delete()
{
    fs::remove_all(GetPath());
}
} // namespace Vultr
