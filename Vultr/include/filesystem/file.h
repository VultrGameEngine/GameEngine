#pragma once
#include <json/json_fwd.hpp>
#include <types/types.hpp>
#include <utils/string/string.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "directory.h"

namespace Vultr
{
#define EXTENSION_MAX 255

    struct IFile
    {
        char *path = nullptr;

        virtual const char *const *get_file_extensions(size_t *size) = 0;
        virtual char *expected_extensions_string() = 0;
    };

    template <const char *const extensions[]>
    struct File : IFile
    {
        static constexpr size_t extensions_length = sizeof(extensions) / sizeof(char *const);

        File()
        {
            path = nullptr;
        }

        File(const char *path)
        {
            size_t len = strlen(path);
            assert(path[len - 1] != '/' && "File ends with a `/`!");

            this->path = str(path);

            strcreplace(this->path, '\\', '/');
        }

        File(const Directory *dir, const char *fname)
        {
            size_t flen = strlen(fname);
            size_t plen = strlen(dir->path);

            assert(fname[flen - 1] != '/' && "File ends with a `/`!");
            assert(fname[0] != '/' && "File name cannot start with a `/`!");

            path = str(dir->path);
            path = strappend(path, fname);

            strcreplace(path, '\\', '/');
        }

        ~File()
        {
            if (path != nullptr)
                free(path);
        }

        const char *const *get_file_extensions(size_t *size) override
        {
            *size = extensions_length;
            return extensions;
        }

        char *expected_extensions_string() override
        {
            char *expected_extensions = str(100);
            for (size_t i = 0; i < extensions_length; i++)
            {
                strcat(expected_extensions, extensions[i]);
            }
            return expected_extensions;
        }

        void operator=(const File &other)
        {
            if (path != nullptr)
                free(path);
            path = str(other.path);
        }
    };

    inline const char *const generic_file_extensions[] = {};
    typedef File<generic_file_extensions> GenericFile;

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
    const char *fbasename(const IFile *file, size_t *len);

    const char *fextension(const IFile *file, bool with_dot = true);

    bool fremove(const IFile *file);

    // Rename a file with the extension
    bool frename(IFile *src, const char *new_name);

    bool fmove(const IFile *src, const IFile *destination);
    bool fmove(IFile *src, const char *destination);

    bool fcopy(IFile *src, const char *dest);

    bool fexists(const IFile *file);

    GenericFile *dirfiles(const Directory *dir);

    // Casts a file (if possible) from one extension type to another
    template <const char *const extensions[]>
    bool cast_file(const GenericFile *src, File<extensions> *dest)
    {
        size_t dest_extension_count = sizeof(extensions) / sizeof(char *const);
        if (dest_extension_count == 0)
        {
            dest->path = str(src->path);
            return true;
        }

        const char *extension = fextension(src);

        // If for whatever reason there is no extension on the generic file then we cannot determine if the destination is valid
        if (extension == nullptr)
        {
            return false;
        }

        for (size_t i = 0; i < dest_extension_count; i++)
        {
            const char *dest_extension = extensions[i];
            if (strcmp(dest_extension, extension) == 0)
            {
                dest->path = str(src->path);
                return true;
            }
        }
        return false;
    }

    u16 fdate_modified(const IFile *file);

    template <const char *const extensions[]>
    bool operator<(const File<extensions> &a, const File<extensions> &b)
    {
        return strcmp(a.path, b.path) < 0;
    }

    template <const char *const extensions[]>
    bool operator==(const File<extensions> &a, const File<extensions> &b)
    {
        return strequal(a.path, b.path);
    }

    void to_json(json &j, const IFile &f);
    void from_json(const json &j, IFile &f);
} // namespace Vultr
