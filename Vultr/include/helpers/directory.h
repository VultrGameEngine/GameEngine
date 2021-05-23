#pragma once
#include <string>
#include "file.h"
#include <set>
#include <filesystem>

namespace Vultr
{
class Directory
{
  public:
    Directory(const std::string &p_path);
    ~Directory() = default;
    std::filesystem::path GetPath() const;

    Directory CreateSubDirectory(const std::string &name);
    File CreateFile(const std::string &name);

    std::set<File> GetFiles() const
    {
        return files;
    }
    std::set<Directory> GetDirectories() const
    {
        return sub_directories;
    }

    std::string GetName() const
    {
        return path.filename().string();
    }

    void Delete();

  private:
    std::filesystem::path path;
    std::set<File> files;
    std::set<Directory> sub_directories;
    friend bool operator<(const Directory &a, const Directory &b);
    friend bool operator==(const Directory &a, const Directory &b);
};
inline bool operator<(const Directory &a, const Directory &b)
{
    return a.path.string() < b.path.string();
}
inline bool operator==(const Directory &a, const Directory &b)
{
    return a.path.string() == b.path.string();
}
inline bool operator!=(const Directory &a, const Directory &b)
{
    return !(a == b);
}
} // namespace Vultr
