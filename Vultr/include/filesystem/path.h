#pragma once
#include <string>
#include <filesystem>

namespace Vultr
{
    void change_working_directory(const char *dir);
    void get_working_directory(char *buf, size_t size);
    // void set_resource_path(const std::string &path);
    // std::string &get_resource_path();
    // std::string get_shortened_resource_path(const std::string &path);
    // std::string GetFullPath(const std::string &path);
    // std::string get_resource_type(const std::string &name);
} // namespace Vultr
