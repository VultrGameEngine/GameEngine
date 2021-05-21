#include <helpers/file.h>
#include <chrono>
#include <iomanip>

namespace Vultr
{
    File::File(const std::string &p_path, const char *p_extensions) : path(p_path), extension(p_extensions)
    {
    }

    std::filesystem::path File::GetPath() const
    {
        return std::filesystem::path(path);
    }

    std::filesystem::file_time_type File::GetDateModified() const
    {
        return std::filesystem::last_write_time(GetPath());
    }

    void File::Delete()
    {
        std::filesystem::remove(GetPath());
    }

    void to_json(json &j, const File &f)
    {
        j["path"] = f.path;
        j["extension"] = std::string(f.extension);
    }
    void from_json(const json &j, File &f)
    {
        f.path = j["path"].get<std::string>();
        f.extension = j["extension"].get<std::string>().c_str();
    }
} // namespace Vultr
