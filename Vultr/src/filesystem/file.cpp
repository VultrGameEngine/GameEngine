#include <helpers/file.h>
#include <chrono>
#include <iomanip>
#include <helpers/path.h>

Vultr::File::File(const std::string &p_path, Extensions extension) : path(p_path)
{
    if (p_path != "")
    {
        if (path.is_relative())
        {
            auto abs = std::filesystem::absolute(path);
            assert(abs != path && "Couldn't get an absolute path! Something went wrong");
            path = abs;
        }
    }
    switch (extension)
    {
    case TEXTURE: {
        expected_extensions = TEXTURE_FILE_EXTENSIONS;
        break;
    }
    case MODEL: {
        expected_extensions = MODEL_FILE_EXTENSIONS;
        break;
    }
    case HEADER_SOURCE: {
        expected_extensions = HEADER_SOURCE_FILE_EXTENSIONS;
        break;
    }
    case HEADER: {
        expected_extensions = HEADER_FILE_EXTENSIONS;
        break;
    }
    case SOURCE: {
        expected_extensions = SOURCE_FILE_EXTENSIONS;
        break;
    }
    case SHADER: {
        expected_extensions = SHADER_FILE_EXTENSIONS;
        break;
    }
    case FONT: {
        expected_extensions = FONT_FILE_EXTENSIONS;
        break;
    }
    case VULTR: {
        expected_extensions = VULTR_FILE_EXTENSIONS;
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
            return file.path.filename().string();
        }
        else
        {
            return file.path.stem().string();
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
    if (!dot && extension.size() > 1)
    {
        return extension.substr(1);
    }
    return extension;
}

Vultr::File::Extensions Vultr::file_get_extension_type(const File &file)
{
    auto extension = file_get_extension(file, true);
    for (auto e : TEXTURE_FILE_EXTENSIONS)
    {
        if (e == extension)
            return File::TEXTURE;
    }
    for (auto e : MODEL_FILE_EXTENSIONS)
    {
        if (e == extension)
            return File::MODEL;
    }
    for (auto e : HEADER_SOURCE_FILE_EXTENSIONS)
    {
        if (e == extension)
            return File::HEADER_SOURCE;
    }
    for (auto e : HEADER_FILE_EXTENSIONS)
    {
        if (e == extension)
            return File::HEADER;
    }
    for (auto e : SOURCE_FILE_EXTENSIONS)
    {
        if (e == extension)
            return File::SOURCE;
    }
    for (auto e : SHADER_FILE_EXTENSIONS)
    {
        if (e == extension)
            return File::SHADER;
    }
    for (auto e : FONT_FILE_EXTENSIONS)
    {
        if (e == extension)
            return File::FONT;
    }
    for (auto e : VULTR_FILE_EXTENSIONS)
    {
        if (e == extension)
            return File::VULTR;
    }
    return File::NONE;
}

bool Vultr::file_exists(const File &file)
{
    return std::filesystem::exists(file.path);
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
    Path base = file.path.parent_path();
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

Vultr::Path Vultr::file_get_relative_path(const File &file)
{
    return std::filesystem::relative(file.path);
}
