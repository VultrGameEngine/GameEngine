#pragma once
#include <string>
#include <filesystem>

namespace Vultr
{
    typedef std::filesystem::path Path;

    void change_working_directory(Path path);
    Path get_working_directory();

    Path convert_windows_path(Path path);
    // void set_resource_path(const std::string &path);
    // std::string &get_resource_path();
    // std::string get_shortened_resource_path(const std::string &path);
    // std::string GetFullPath(const std::string &path);
    // std::string get_resource_type(const std::string &name);
} // namespace Vultr
