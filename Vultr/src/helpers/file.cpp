#include <helpers/file.h>
#include <chrono>
#include <iomanip>

Vultr::File::File(const std::string &p_path, Extensions extension) : path(p_path)
{
    switch (extension)
    {
    case TEXTURE: {
        expected_extensions = {".jpeg", ".jpg", ".png", ".bmp", ".dds"};
        break;
    }
    case MODEL: {
        expected_extensions = {".obj", ".fbx", ".blend"};
        break;
    }
    case HEADER_SOURCE: {
        expected_extensions = {".h", ".hpp", ".c", ".cpp", ".cc"};
        break;
    }
    case HEADER: {
        expected_extensions = {".h", ".hpp"};
        break;
    }
    case SOURCE: {
        expected_extensions = {".c", ".cpp", ".cc"};
        break;
    }
    case SHADER: {
        expected_extensions = {".glsl"};
        break;
    }
    case FONT: {
        expected_extensions = {".ttf"};
        break;
    }
    case NONE:
        expected_extensions = {};
        break;
    default:
        break;
    }
}

std::string Vultr::file_get_name(const File &file, bool extension)
{
    if (file.path.has_filename())
    {
        if (extension)
        {
            return file.path.filename();
        }
        else
        {
            return file.path.stem();
        }
    }
    else
    {
        return "";
    }
}
bool Vultr::file_has_extension(const File &file)
{
    return file.path.has_extension();
}
std::string Vultr::file_get_extension(const File &file, bool dot)
{
    auto extension = file.path.extension().string();
    if (extension.size() > 1)
    {
        return extension.substr(1);
    }
    return extension;
}

bool Vultr::file_extension_matches(const File &file, const File &other)
{
    if (file.expected_extensions.size() == 0)
        return true;

    if (!file_has_extension(other))
        return false;

    auto other_extension = file_get_extension(other, false);

    for (auto e : file.expected_extensions)
    {
        if (e == other_extension)
            return true;

        // Strip the dot just in case we accidentally add it to the file
        // This shouldn't happen in the first place, but just in case someone is an idiot...
        if (e[0] == '.' && e.size() > 1)
        {
            if (e.substr(1) == other_extension)
                return true;
        }
    }
    return false;
}
std::string Vultr::file_get_expected_extension_string(const File &file)
{
    return "";
}

void Vultr::file_rename(const File &file, const char *name)
{
    std::filesystem::path base = file.path.parent_path();
    std::filesystem::rename(file.path, base / name);
}

bool Vultr::delete_file(const File &file)
{
    return std::filesystem::remove(file.path);
}

std::filesystem::file_time_type Vultr::file_get_date_modified(const File &file)
{
    return std::filesystem::last_write_time(file.path);
}
