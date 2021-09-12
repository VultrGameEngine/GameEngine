#include <filesystem/file.h>
#include <filesystem/directory.h>
#include <sys/stat.h>

#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include <dirent.h>

namespace Vultr
{
    static char *path_basename(char *path, size_t *len)
    {
        size_t _len = strlen(path);
        if (_len < 2)
        {
            *len = _len;
            return path;
        }

        if (_len == 2)
        {
            if (strequal(path, "./"))
            {
                *len = 2;
                return path;
            }
        }

        size_t prev_slash_loc = _len;
        for (s32 i = _len - 1; i >= 0; i--)
        {
            char c = path[i];
            if (c == '/')
            {
                if (i < _len - 1)
                {
                    *len = prev_slash_loc - i - 1;
                    return path + i + 1;
                }
                else
                {
                    prev_slash_loc = i;
                }
            }
        }

        *len = _len;
        return path;
    }

    static size_t path_dirname(char *path)
    {
        size_t len = strlen(path);
        for (s32 i = len - 1; i >= 0; i--)
        {
            char c = path[i];
            if (c == '/')
            {
                if (i < len - 1)
                {
                    return i;
                }
            }
        }
        return 0;
    }

    const char *fbasename(const IFile *file, size_t *len)
    {
        char *path = file->path;
        size_t path_len = strlen(path);
        if (path_len < 3)
        {
            assert(!strequal(path, ".") && "File `.` does not have a file name!");
            assert(!strequal(path, "./") && "File `./` does not have a file name!");
            assert(!strequal(path, "/") && "File `/` does not have a file name!");
            assert(path[path_len - 1] == '/' && "File ends with a `/`!");
        }

        return path_basename(path, len);
    }

    const char *fextension(const IFile *file, bool with_dot)
    {
        size_t len;
        const char *basename = fbasename(file, &len);
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
        size_t len;
        const char *path = src->path;
        const char *basename = fbasename(src, &len);

        size_t path_size = strlen(path) - len;
        size_t new_path_len = path_size + strlen(new_name);

        char *new_path = str(new_path_len);

        strncat(new_path, path, path_size);
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

    static char *dir_path(const char *path)
    {
        size_t len = strlen(path);

        // If there is a trailing slash, delete it
        bool append_slash = false;
        if (path[len - 1] != '/' && path[len - 1] != '\\')
        {
            len += 1;
            append_slash = true;
        }
        char *res_path = strn(path, len);
        if (append_slash)
        {
            res_path[len - 1] = '/';
        }

        strcreplace(res_path, '\\', '/');
        return res_path;
    }

    Directory::Directory(const char *path)
    {
        this->path = dir_path(path);
    }

    Directory::Directory(const Directory *dir, const char *path)
    {
        size_t len = strlen(dir->path) + strlen(path);

        char combined_path[len + 1];
        for (u16 i = 0; i < len + 1; i++)
        {
            combined_path[i] = '\0';
        }

        strcpy(combined_path, dir->path);
        strcat(combined_path, path);

        this->path = dir_path(combined_path);
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
                    // If the directory already exists, we can ignore the error because
                    // this is normal and expected. However if the error is something
                    // else we should fail and return
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

    const char *dirbasename(const Directory *dir, size_t *len)
    {
        char *path = dir->path;
        return path_basename(path, len);
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
        size_t len = path_dirname(dir->path);

        // If the length is 0, that means that the user likely forgot to put a "./"
        // indicating the working directory i.e. `some_path` is the same thing as
        // `./some_path`
        if (len == 0)
        {
            *parent = Directory("./");
        }
        else
        {
            char str[len + 1];
            strncpy(str, dir->path, len);
            str[len] = '\0';
            *parent = Directory(str);
        }
    }

    void dirparent(const IFile *file, Directory *parent)
    {
        size_t len = path_dirname(file->path);

        // If the length is 0, that means that the user likely forgot to put a "./"
        // indicating the working directory i.e. `some_path` is the same thing as
        // `./some_path`
        if (len == 0)
        {
            *parent = Directory("./");
        }
        else
        {
            char str[len + 1];
            strncpy(str, file->path, len);
            str[len] = '\0';
            *parent = Directory(str);
        }
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
        size_t len;
        const char *path = dir->path;
        const char *basename = dirbasename(dir, &len);

        size_t path_size = path_dirname(dir->path) + 1;
        size_t new_path_len = path_size + strlen(name);

        char new_path[new_path_len + 1];
        for (u16 i = 0; i < new_path_len + 1; i++)
        {
            new_path[i] = '\0';
        }

        strncpy(new_path, path, path_size);
        strcat(new_path, name);
        new_path[new_path_len] = '\0';

        auto out_dir = Directory(new_path);

        bool successful = rename(path, out_dir.path) == 0;

        if (successful)
        {
            *dir = Directory(new_path);
        }

        return successful;
    }

    bool dirmove(Directory *src, const char *dest)
    {
        size_t len;
        const char *basename = dirbasename(src, &len);

        size_t path_size = strlen(dest) + strlen(basename);

        char new_path[path_size + 1];

        for (u16 i = 0; i < path_size + 1 + 1; i++)
        {
            new_path[i] = '\0';
        }

        strcpy(new_path, dest);
        strcat(new_path, basename);
        new_path[path_size] = '\0';

        auto out_dir = Directory(new_path);

        bool succcessful = rename(src->path, new_path) == 0;
        if (succcessful)
        {
            *src = Directory(new_path);
        }

        return succcessful;
    }

    bool dirmove(Directory *src, const Directory *dest)
    {
        return dirmove(src, dest->path);
    }

    // bool dircopy(Directory *src, const char *dest)
    // {
    // }
    static u32 count_dir_type(const char *path, unsigned char type)
    {
        u32 count = 0;
        DIR *odir;
        struct dirent *entry;

        odir = opendir(path);
        assert(odir != nullptr && "Failed to open directory!");
        while ((entry = readdir(odir)) != nullptr)
        {
            if (entry->d_type == type)
            {
                if (type == DT_DIR)
                {
                    if (strequal(entry->d_name, ".") || strequal(entry->d_name, ".."))
                        continue;
                }
                count++;
            }
        }
        closedir(odir);
        return count;
    }

    u32 dirfilecount(const Directory *dir)
    {
        return count_dir_type(dir->path, DT_REG);
    }

    u32 dirsubdirectorycount(const Directory *dir)
    {
        return count_dir_type(dir->path, DT_DIR);
    }

    // GenericFile *dirfiles(const Directory *dir)
    // {
    // }

    // Directory *dirsubdirs(const Directory *dir)
    // {
    // }

} // namespace Vultr
