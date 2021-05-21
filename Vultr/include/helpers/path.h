#pragma once
#include <string>

namespace Vultr::Path
{
    void set_resource_path(const std::string &path);
    std::string &get_resource_path();
    std::string get_shortened_resource_path(const std::string &path);
    std::string GetFullPath(const std::string &path);

} // namespace Vultr::Path
