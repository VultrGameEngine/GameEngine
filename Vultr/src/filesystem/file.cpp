#include <filesystem/file.h>

namespace Vultr
{
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

    bool fexists(IFile *file)
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
} // namespace Vultr
