#include <helpers/file.h>
#include <chrono>
#include <iomanip>

namespace Vultr
{
File::File(const std::string &p_path) : path(p_path)
{
}

std::filesystem::path File::GetPath() const
{
    return path;
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
