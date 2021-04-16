#pragma once
#include <string>
#include <filesystem>

namespace Vultr
{
class File
{
  public:
    File(const std::string &p_path);
    ~File() = default;
    std::filesystem::path GetPath() const;

    std::filesystem::file_time_type GetDateModified() const;

    std::string GetName() const
    {
        return path.filename();
    }

    void Delete();

  private:
    std::filesystem::path path;
    friend bool operator<(const File &a, const File &b);
    friend bool operator==(const File &a, const File &b);
};
inline bool operator<(const File &a, const File &b)
{
    return a.path.string() < b.path.string();
}
inline bool operator==(const File &a, const File &b)
{
    return a.path.string() == b.path.string();
}
} // namespace Vultr
