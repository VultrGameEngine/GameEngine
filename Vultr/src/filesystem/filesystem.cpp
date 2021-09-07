#include <filesystem/file.h>
#include <filesystem/directory.h>
#include <sys/stat.h>

#include <stdio.h>
#include <ftw.h>
#include <unistd.h>

namespace Vultr
{
    static char *path_basename(char *path, size_t &len)
    {
        size_t _len = strlen(path);
        for (size_t i = _len - 1; i >= 0; i--)
        {
            char c = path[i];
            if (c == '/')
            {
            }
        }
    }

    const char *fbasename(const IFile *file)
    {
        char *path = file->path;
        return basename(path);
    }

    const char *fextension(const IFile *file, bool with_dot)
    {
        const char *basename = fbasename(file);
        const char *extension = strchr(basename, '.');

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

    bool fremove(const IFile *file)
    {
        return remove(file->path) == 0;
    }

    bool frename(IFile *src, const char *new_name)
    {
        const char *path = src->path;
        const char *basename = fbasename(src);

        size_t path_size = strlen(path) - strlen(basename);
        size_t new_path_len = path_size + strlen(new_name);

        char *new_path = str(new_path_len);
        strcat(new_path, new_name);

        bool successful = rename(path, new_path) == 0;
        if (successful)
        {
            free(src->path);
            src->path = new_path;
        }
        else
        {
            free(new_path);
        }

        return successful;
    }

    bool fmove(const IFile *src, const IFile *destination)
    {
        return rename(src->path, destination->path) == 0;
    }

    bool fmove(IFile *src, const char *destination)
    {
        bool successful = rename(src->path, destination) == 0;

        if (successful)
        {
            free(src->path);
            src->path = str(destination);
        }
        return successful;
    }

    bool fcopy(IFile *src, const char *dest)
    {
        FILE *f_src = fopen(src->path, "r");
        if (f_src == nullptr)
        {
            fprintf(stderr, "Failed to open file %s", src->path);
            fclose(f_src);
            return false;
        }

        FILE *f_dest = fopen(dest, "w");
        if (f_dest == nullptr)
        {
            fprintf(stderr, "Failed to open file %s", dest);
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
                fwrite(buffer, 1, bytes, f_dest);
        }

        free(src->path);
        src->path = str(dest);

        fclose(f_src);
        fclose(f_dest);

        return true;
    }

    bool fexists(const IFile *file)
    {
        struct stat buffer;
        return (stat(file->path, &buffer) == 0);
    }

    u16 fdate_modified(const IFile *file)
    {
        struct stat time;
        lstat(file->path, &time);
        return time.st_mtim.tv_sec * 1000;
    }

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

    const char *dirbasename(const Directory *dir)
    {
        char *path = dir->path;
        return basename(path);
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
        // const char *parent_path = dirname(dir->path);
        // *parent = Directory(parent_path);
    }

    void dirparent(const IFile *file, Directory *parent)
    {
        // const char *parent_path = dirname(file->path);
        // *parent = Directory(parent_path);
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

    bool dirrename(Directory *dir, const char *name)
    {
        const char *path = dir->path;
        const char *basename = dirbasename(dir);

        size_t path_size = strlen(path) - strlen(basename);
    }

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
