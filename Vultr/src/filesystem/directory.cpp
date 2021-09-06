#include <filesystem/directory.h>
#include <libgen.h>
#include <sys/stat.h>

#include <stdio.h>
#include <ftw.h>
#include <unistd.h>

namespace Vultr
{
    Directory::Directory(const char *path)
    {
        size_t len = strlen(path);

        // If there is a trailing slash, delete it
        bool append_slash = false;
        if (path[len - 1] != '/')
        {
            len += 1;
            append_slash = true;
        }

        this->path = strn(path, len);
        if (append_slash)
        {
            this->path[len - 1] = '/';
        }
    }

    Directory::~Directory()
    {
        if (path != nullptr)
        {
            free(path);
        }
    }

    void Directory::operator=(const Directory &other)
    {
        if (path != nullptr)
            free(path);
        path = str(other.path);
    }

    static s8 mkdir_p(const char *path, u32 mode)
    {
        const size_t len = strlen(path);

        char _path[len + 1];
        strcpy(_path, path);

        // Iterate the string
        for (char *p = _path + 1; *p; p++)
        {
            if (*p == '/')
            {
                // Temporarily truncate
                *p = '\0';

                if (mkdir(_path, mode) != 0)
                {
                    // If the directory already exists, we can ignore the error because this is normal and expected. However if the error is something else we should fail and return
                    if (errno != EEXIST)
                        return -1;
                }

                *p = '/';
            }
        }

        if (mkdir(_path, mode) != 0)
        {
            if (errno != EEXIST)
                return -1;
        }

        return 0;
    }

    bool dirmake(const char *path, Directory *dir)
    {
        bool res = mkdir_p(path, S_IRWXU | S_IRWXG | S_IRWXO) == 0;

        if (res)
        {
            *dir = Directory(path);
        }
        else
        {
            dir->path = nullptr;
        }

        return res;
    }

    bool direxists(const Directory *dir)
    {
        struct stat st = {0};
        return stat(dir->path, &st) == 0;
    }

    void dirparent(const Directory *dir, Directory *parent)
    {
        const char *parent_path = dirname(dir->path);
        *parent = Directory(parent_path);
    }

    void dirparent(const IFile *file, Directory *parent)
    {
        const char *parent_path = dirname(file->path);
        *parent = Directory(parent_path);
    }

    static s32 on_rm_dir_cb(const char *fpath, const struct stat *sb, s32 typeflag, struct FTW *ftwbuf)
    {
        s32 rv = remove(fpath);

        if (rv)
            perror(fpath);

        return rv;
    }

    bool dirremove(const Directory *dir)
    {
        return nftw(dir->path, on_rm_dir_cb, 64, FTW_DEPTH | FTW_PHYS) == 0;
    }

    // bool dirrename(Directory *dir, const char *name)
    // {
    // }

    // bool dirmove(Directory *src, const Directory *dest)
    // {
    // }
    // bool dirmove(Directory *src, const char *dest)
    // {
    // }

    // bool dircopy(Directory *src, const char *dest)
    // {
    // }
} // namespace Vultr
