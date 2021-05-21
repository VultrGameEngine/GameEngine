#pragma once
#include <string>
#include <filesystem>
#include <json/json.hpp>

namespace Vultr
{
#define TEXTURE_FILE_EXTENSIONS "Image (*.jpg *.png...){.jpeg,.jpg,.png,.bmp,.dds,.JPEG,.JPG,.PNG,.BMP,.DDS}"
#define HEADER_SOURCE_FILE_EXTENSIONS "Header/Source (*.h,*.cpp...){.h,.hpp,.c,.cpp,.cc}"
#define HEADER_FILE_EXTENSIONS "Header (*.h *.hpp){.h,.hpp}"
#define SOURCE_FILE_EXTENSIONS "Source (*.c *.cpp *.cc) {.c,.cpp,.cc}"
#define SHADER_FILE_EXTENSIONS "Shader (*.glsl) {.glsl}"
    class File
    {
      public:
        File() = default;
        File(const std::string &p_path, const char *p_extension = nullptr);
        ~File() = default;
        std::filesystem::path GetPath() const;

        std::filesystem::file_time_type GetDateModified() const;

        std::string GetName() const
        {
            return path.filename();
        }

        const char *GetExtension() const
        {
            return extension;
        }

        void Delete();

      private:
        std::filesystem::path path;
        const char *extension;
        friend bool operator<(const File &a, const File &b);
        friend bool operator==(const File &a, const File &b);

        friend void to_json(json &j, const File &f);
        friend void from_json(const json &j, File &f);
    };

    inline bool operator<(const File &a, const File &b)
    {
        return a.path.string() < b.path.string();
    }
    inline bool operator==(const File &a, const File &b)
    {
        return a.path.string() == b.path.string();
    }

    void to_json(json &j, const File &f);
    void from_json(const json &j, File &f);
} // namespace Vultr
