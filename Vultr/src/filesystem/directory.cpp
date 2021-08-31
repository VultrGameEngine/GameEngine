#include <helpers/directory.h>
#include <filesystem>

static void directory_cache_files(Vultr::Directory &dir)
{
    dir.files.clear();
    dir.sub_directories.clear();
    for (auto &file : std::filesystem::directory_iterator(dir.path))
    {
        if (file.is_directory())
        {
            dir.sub_directories.push_back(Vultr::Directory(file.path().string()));
        }
        else
        {
            dir.files.push_back(Vultr::File(file.path().string()));
        }
    }
    dir.cached_files = true;
}

Vultr::Directory::Directory(const std::string &p_path) : path(p_path)
{
    if (path.is_relative())
    {
        auto abs = std::filesystem::absolute(path);
        assert(abs != path && "Couldn't get an absolute path! Something went wrong");
        path = abs;
    }
}

std::string Vultr::directory_get_name(const Directory &dir)
{
    return dir.path.filename().string();
}

u32 Vultr::directory_get_number_files(Directory &dir, bool use_cache)
{
    if (!use_cache || !dir.cached_files)
        directory_cache_files(dir);
    return dir.files.size() + dir.sub_directories.size();
}

std::vector<Vultr::File> Vultr::directory_get_files(Directory &dir, bool use_cache)
{
    if (!use_cache || !dir.cached_files)
        directory_cache_files(dir);
    return dir.files;
}

std::vector<Vultr::Directory> Vultr::directory_get_sub_directories(Directory &dir, bool use_cache)
{
    if (!use_cache || !dir.cached_files)
        directory_cache_files(dir);
    return dir.sub_directories;
}
Vultr::Path Vultr::file_get_relative_path(const Directory &dir)
{
    return std::filesystem::relative(dir.path);
}

bool Vultr::delete_directory(Directory &dir)
{
    auto res = std::filesystem::remove_all(dir.path);
    if (res)
        directory_cache_files(dir);
    return res;
}

void Vultr::rename_directory(Directory &dir, const char *name)
{
    Directory new_dir = Directory((dir.path / std::string(name)).string());
    std::filesystem::rename(dir.path, new_dir.path);
    directory_cache_files(dir);
}

void Vultr::move_file(File &file, Directory &dir)
{
    auto new_path = dir.path / file_get_name(file);
    std::filesystem::rename(file.path, new_path);
    file.path = new_path;
}

Vultr::Directory Vultr::create_sub_directory(Directory &dir, const char *name)
{
    Directory new_dir = Directory((dir.path / name).string());
    std::filesystem::create_directory(new_dir.path);
    return new_dir;
}
Vultr::File Vultr::create_file(Directory &dir, const char *name)
{
    return File((dir.path / name).string());
}

Vultr::Directory Vultr::directory_get_parent_directory(const Directory &directory)
{
    return Directory(directory.path.parent_path().string());
}

Vultr::Directory Vultr::directory_get_root_directory(const Directory &directory)
{
    return Directory(directory.path.root_directory().string());
}

Vultr::Directory Vultr::file_get_parent_directory(const File &file)
{
    return Directory(file.path.parent_path().string());
}

Vultr::Directory Vultr::file_get_root_directory(const File &file)
{
    return Directory(file.path.root_directory().string());
}
