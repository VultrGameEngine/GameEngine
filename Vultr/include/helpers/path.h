#pragma once
#include <string>

namespace Vultr::Path
{
#define SHADER_FILE "shader"
#define HEADER_FILE "header"
#define SOURCE_FILE "source"
#define IMAGE_FILE "image"
#define MODEL_FILE "3d_model"
#define UNKNOWN_FILE "unknown"
    void set_resource_path(const std::string &path);
    std::string &get_resource_path();
    std::string get_shortened_resource_path(const std::string &path);
    std::string GetFullPath(const std::string &path);
    std::string get_resource_type(const std::string &name);

} // namespace Vultr::Path
