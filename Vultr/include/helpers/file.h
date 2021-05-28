#pragma once
#include <string>
#include <filesystem>
#include <json/json_fwd.hpp>
#include <types/types.hpp>

namespace Vultr
{
#define TEXTURE_FILE_EXTENSIONS "Image (*.jpg *.png...){.jpeg,.jpg,.png,.bmp,.dds,.JPEG,.JPG,.PNG,.BMP,.DDS}"
#define MODEL_3D_FILE_EXTENSIONS "Image (*.obj *.fbx...){.obj,.fbx,.blend,}"
#define HEADER_SOURCE_FILE_EXTENSIONS "Header/Source (*.h,*.cpp...){.h,.hpp,.c,.cpp,.cc}"
#define HEADER_FILE_EXTENSIONS "Header (*.h *.hpp){.h,.hpp}"
#define SOURCE_FILE_EXTENSIONS "Source (*.c *.cpp *.cc) {.c,.cpp,.cc}"
#define SHADER_FILE_EXTENSIONS "Shader (*.glsl) {.glsl}"
    class File
    {
      public:
        File() = default;
        File(const std::string &p_path, const std::string &p_extension = ".*");
        ~File() = default;
        std::filesystem::path GetPath() const;

        std::filesystem::file_time_type GetDateModified() const;

        std::string GetName() const
        {
            return std::filesystem::path(path).filename().string();
        }

        const char *GetExtension() const
        {
            return extension.c_str();
        }

        bool ExtensionMatches(const File &other)
        {
            std::string other_extension = other.GetName();
            s32 dot = other_extension.rfind(".");
            if (dot == std::string::npos)
                return false;
            other_extension = other_extension.substr(dot + 1);

            std::vector<std::string> extensions;
            s32 first_bracket = extension.find("{");
            if (first_bracket == std::string::npos)
                return true;
            s32 last_bracket = extension.rfind("}");
            if (last_bracket == std::string::npos)
                return true;

            std::string list = extension.substr(first_bracket + 1, last_bracket - 1);
            std::string current_string = "";
            u32 i = 0;
            for (auto c : list)
            {
                if (c == ',')
                {
                    extensions.push_back(current_string);
                    current_string = "";
                }
                else if (c != '.')
                {
                    current_string += c;
                    if (i == current_string.size() - 1)
                    {
                        extensions.push_back(current_string);
                        current_string = "";
                    }
                }
                i++;
            }

            for (auto e : extensions)
            {
                if (e == other_extension)
                    return true;
            }
            return false;
        }

        void Delete();

        std::string path;
        std::string extension = ".*";
        friend bool operator<(const File &a, const File &b);
        friend bool operator==(const File &a, const File &b);

        friend void to_json(json &j, const File &f);
        friend void from_json(const json &j, File &f);
    };

    inline bool operator<(const File &a, const File &b)
    {
        return a.path < b.path;
    }
    inline bool operator==(const File &a, const File &b)
    {
        return a.path == b.path;
    }

    void to_json(json &j, const File &f);
    void from_json(const json &j, File &f);
} // namespace Vultr
