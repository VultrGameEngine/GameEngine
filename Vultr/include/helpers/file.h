#pragma once
#include <string>
#include <filesystem>
#include <json/json_fwd.hpp>
#include <types/types.hpp>

namespace Vultr
{
    struct File
    {
        enum Extensions
        {
            TEXTURE,
            MODEL,
            HEADER_SOURCE,
            HEADER,
            SOURCE,
            SHADER,
            NONE
        };
        File() = default;
        File(const std::string &p_path, Extensions extension = NONE);
        std::filesystem::path path;
        // Mainly for editor, stores what extensions are required in this file
        // Used for determining if files provided will actually work for this file type (i.e. if a png is provided to a shader file type, it won't work)
        //
        // This is separate from the file_get_extension function which will get the current extension of the file
        // Should NOT include dot
        std::vector<std::string> expected_extensions;
    };

    struct TextureSource : File
    {
        TextureSource() : File("", TEXTURE){};
        TextureSource(const std::string &p_path) : File(p_path, TEXTURE){};
    };

    struct ModelSource : File
    {
        ModelSource() : File("", MODEL){};
        ModelSource(const std::string &p_path) : File(p_path, MODEL){};
    };

    struct HeaderFile : File
    {
        HeaderFile() : File("", HEADER){};
        HeaderFile(const std::string &p_path) : File(p_path, HEADER){};
    };

    struct SourceFile : File
    {
        SourceFile() : File("", SOURCE){};
        SourceFile(const std::string &p_path) : File(p_path, SOURCE){};
    };

    struct HeaderAndSourceFile : File
    {
        HeaderAndSourceFile() : File("", HEADER_SOURCE){};
        HeaderAndSourceFile(const std::string &p_path) : File(p_path, HEADER_SOURCE){};
    };

    struct ShaderSource : File
    {
        ShaderSource() : File("", SHADER){};
        ShaderSource(const std::string &p_path) : File(p_path, SHADER){};
    };

    // Gets the name of the file (can be specified whether this includes the extension or not)
    std::string file_get_name(const File &file, bool extension = true);
    bool file_has_extension(const File &file);

    // Returns with dot
    std::string file_get_extension(const File &file, bool dot = true);
    // Used for editor, will compare expected_extensions of file1 to the real extension of file2 if it has an extension
    // It does NOT compare the real file extensions of both files to see if they match, there is no function to do this. Simply compare file_get_extension to do this
    bool file_extension_matches(const File &file1, const File &file2);

    // Used for editor, just for the built in explorer
    std::string file_get_expected_extension_string(const File &file);

    bool delete_file(const File &file);
    std::filesystem::file_time_type file_get_date_modified(const File &file);

    void file_rename(const File &file, const char *name);

    // #define TEXTURE_FILE_EXTENSIONS "Image (*.jpg *.png...){.jpeg,.jpg,.png,.bmp,.dds,.JPEG,.JPG,.PNG,.BMP,.DDS}"
    // #define MODEL_3D_FILE_EXTENSIONS "Image (*.obj *.fbx...){.obj,.fbx,.blend,}"
    // #define HEADER_SOURCE_FILE_EXTENSIONS "Header/Source (*.h,*.cpp...){.h,.hpp,.c,.cpp,.cc}"
    // #define HEADER_FILE_EXTENSIONS "Header (*.h *.hpp){.h,.hpp}"
    // #define SOURCE_FILE_EXTENSIONS "Source (*.c *.cpp *.cc) {.c,.cpp,.cc}"
    // #define SHADER_FILE_EXTENSIONS "Shader (*.glsl) {.glsl}"

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
