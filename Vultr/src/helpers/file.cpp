#include <helpers/file.h>
#include <chrono>
#include <iomanip>

namespace Vultr
{
    File::File(const std::string &p_path, const std::string &p_extensions) : path(p_path), extension(p_extensions)
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

} // namespace Vultr
