#pragma once
#include <json/json_fwd.hpp>
#include <types/types.hpp>
#include "path.h"
#include <libgen.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace Vultr
{
#define EXTENSION_MAX 255

    struct IFile
    {
        char *path;

        virtual const char *const *get_file_extensions(size_t *size);
    };

    template <const char *const extensions[]>
    struct File : IFile
    {
        File(const char *path)
        {
            u32 size = strlen(path);
            this->path = static_cast<char *>(malloc(sizeof(char) * size));

            strcpy(this->path, path);
        }
        ~File()
        {
            free(this->path);
        }

        const char *const *get_file_extensions(size_t *size) override
        {
            *size = sizeof(extensions);
            return extensions;
        }

        void to_json(json &j, const File &f);
        void from_json(const json &j, File &f);
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

    // Get the name of the file, not the path.
    template <const char *const extensions[]>
    char *basename(const File<extensions> *file)
    {
        return basename(file->path);
    }

    template <const char *const extensions[]>
    char *extension(const File<extensions> *file, bool with_dot = true)
    {
        char *basename = basename(file);
        char *extension = strchr(basename, '.');

        // If no pointer was found, then just return
        if (!extension)
            return nullptr;

        if (with_dot)
        {
            return extension;
        }
        else
        {
            return extension + 1;
        }
    }

    template <const char *const extensions[]>
    bool rm_file(const File<extensions> *file)
    {
        return remove(file->path) == 0;
    }

    template <const char *const extensions[]>
    bool mv_file(const File<extensions> *src, const File<extensions> *destination)
    {
        return rename(src->path, destination->path) == 0;
    }

    template <const char *const extensions[]>
    bool cp_file(const File<extensions> *source, const File<extensions> *destination)
    {
        FILE *f_src = fopen(source->path, "r");
        if (f_src == nullptr)
        {
            fprintf(stderr, "Failed to open file %s", source->path);
            fclose(f_src);
            return false;
        }

        FILE *f_dest = fopen(destination->path, "w");
        if (f_dest == nullptr)
        {
            fprintf(stderr, "Failed to open file %s", destination->path);
            fclose(f_src);
            fclose(f_dest);
            return false;
        }

        // 32 kB buffer
#define READ_BUFFER_SIZE 32768
        char buffer[READ_BUFFER_SIZE];

        while (!feof(f_src))
        {
            // Read contents from source
            size_t bytes = fread(buffer, 1, sizeof(buffer), f_src);

            // If there are any bytes read, then write them to the destination file
            if (bytes)
                fwrite(f_dest, 1, bytes, f_dest);
        }

        return true;
    }

    // Used for editor, will compare expected_extensions of file1 to the real extension of file2 if it has an extension
    // It does NOT compare the real file extensions of both files to see if they match, there is no function to do this. Simply compare file_get_extension to do this
    bool file_extension_matches(const File &file1, const File &file2);

    std::filesystem::file_time_type file_get_date_modified(const File &file);

    void file_rename(const File &file, const char *name);

    bool operator<(const File &a, const File &b)
    {
        return a.path < b.path;
    }

    bool operator==(const File &a, const File &b)
    {
        return a.path == b.path;
    }
} // namespace Vultr
