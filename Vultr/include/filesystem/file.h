#pragma once
#include <json/json_fwd.hpp>
#include <types/types.hpp>
#include "path.h"
#include <libgen.h>

namespace Vultr
{
#define EXTENSION_MAX 255

    struct IFile
    {
        virtual const char *const *get_file_extensions(size_t *size);
    };

    template <const char *const extensions[]>
    struct File : IFile
    {
        char *path;

        File(const char *path, size_t size)
        {
        }
        ~File()
        {
            free(path);
        }
    };

    inline const char *const flex_file_extensions[] = {};
    typedef File<flex_file_extensions> FlexFile;

    inline const char *const texture_source_extensions[] = {".jpeg", ".jpg", ".png", ".bmp", ".dds"};
    typedef File<texture_source_extensions> TextureSource;

    inline const char *const model_source_extensions[] = {".obj", ".fbx", ".blend"};
    typedef File<model_source_extensions> ModelSource;

    inline const char *const header_source_extensions[] = {".h", ".hpp", ".c", ".cpp", ".cc"};
    typedef File<header_source_extensions> HeaderAndSourceFile;

    inline const char *const header_extensions[] = {".h", ".hpp"};
    typedef File<header_extensions> HeaderFile;

    inline const char *const source_extensions[] = {".c", ".cpp", ".cc"};
    typedef File<source_extensions> SourceFile;

    inline const char *const shader_extensions[] = {".glsl"};
    typedef File<shader_extensions> ShaderSource;

    inline const char *const font_source_extensions[] = {".ttf"};
    typedef File<font_source_extensions> FontSource;

    inline const char *const vultr_source_extensions[] = {".vultr"};
    typedef File<vultr_source_extensions> VultrSource;

    // Get the name of the file, not the path. You can specify whether this includes the extension with the `with_extension` parameter, default is true.
    template <const char *const extensions[]>
    void basename(const File<extensions> *file, char *buf, bool with_extension = true)
    {
    }

    void get_extension(char *buf)
    {
    }

    const char *const *get_file_extensions(size_t *size) override
    {
        *size = sizeof(extensions);
        return extensions;
    }

    // Gets the name of the file (can be specified whether this includes the extension or not)
    std::string file_get_name(const File &file, bool extension = true);
    bool file_has_extension(const File &file);
    bool file_exists(const File &file);

    // Returns with dot
    std::string file_get_extension(const File &file, bool dot = true);
    File::Extensions file_get_extension_type(const File &file);
    // Used for editor, will compare expected_extensions of file1 to the real extension of file2 if it has an extension
    // It does NOT compare the real file extensions of both files to see if they match, there is no function to do this. Simply compare file_get_extension to do this
    bool file_extension_matches(const File &file1, const File &file2);

    // Used for editor, just for the built in explorer
    std::string file_get_expected_extension_string(const File &file);

    bool delete_file(const File &file);
    std::filesystem::file_time_type file_get_date_modified(const File &file);

    void file_rename(const File &file, const char *name);
    Path file_get_relative_path(const File &file);

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
