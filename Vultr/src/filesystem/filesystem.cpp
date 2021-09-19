#include <filesystem/virtual_filesystem.h>
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

    bool fextension_matches(const char *extension, const char *const file_type[], size_t len)
    {
        for (s32 i = 0; i < len; i++)
        {
            if (strequal(extension, file_type[i]))
            {
                return true;
            }
        }
        return false;
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

    bool fmove(IFile *src, const Directory *destination)
    {
        size_t len;
        const char *basename = fbasename(src, &len);

        auto dest_file = GenericFile(destination, basename);
        bool successful = rename(src->path, dest_file.path) == 0;

        if (successful)
        {
            free(src->path);
            src->path = str(dest_file.path);
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

    bool dircurrentworking(Directory *dir)
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == nullptr)
        {
            return false;
        }

        *dir = Directory(cwd);
        return true;
    }

    bool dirchangeworking(const Directory *dir)
    {
        return chdir(dir->path) == 0;
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

    bool dirmake(const Directory *dir)
    {
        bool res = mkdir_p(dir->path, S_IRWXU | S_IRWXG | S_IRWXO) == 0;
        return res;
    }

    bool direxists(const Directory *dir)
    {
        struct stat st = {0};
        return stat(dir->path, &st) == 0;
    }

    bool dirhasparent(const Directory *dir)
    {
        size_t len = path_dirname(dir->path);
        if (len == 0)
        {
            return false;
        }
        else if (len == 1 && dir->path[0] == '.')
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void dirparent(const Directory *dir, Directory *parent)
    {
        size_t len = path_dirname(dir->path);

        // If the length is 0, that means that the user likely forgot to put a "./"
        // indicating the working directory i.e. `some_path` is the same thing as
        // `./some_path`
        if (len == 0)
        {
            if (dir->path[0] == '/')
            {
                *parent = Directory("/");
            }
            else
            {
                *parent = Directory("./");
            }
        }
        else if (len == 1 && dir->path[0] == '.')
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

    GenericFile *dirfiles(const Directory *dir, size_t *len)
    {
        size_t file_count = dirfilecount(dir);
        *len = file_count;

        GenericFile *files = new GenericFile[file_count];

        DIR *odir;
        struct dirent *entry;

        odir = opendir(dir->path);
        assert(odir != nullptr && "Failed to open directory!");

        u16 i = 0;
        while ((entry = readdir(odir)) != nullptr)
        {
            if (entry->d_type == DT_REG)
            {
                files[i] = GenericFile(dir, entry->d_name);
                i++;
            }
        }
        closedir(odir);
        return files;
    }

    Directory *dirsubdirs(const Directory *dir, size_t *len)
    {
        size_t dir_count = dirsubdirectorycount(dir);
        *len = dir_count;

        Directory *dirs = new Directory[dir_count];

        DIR *odir;
        struct dirent *entry;

        odir = opendir(dir->path);
        assert(odir != nullptr && "Failed to open directory!");

        u16 i = 0;
        while ((entry = readdir(odir)) != nullptr)
        {
            if (entry->d_type == DT_DIR)
            {
                if (strequal(entry->d_name, ".") || strequal(entry->d_name, ".."))
                    continue;
                dirs[i] = Directory(dir, entry->d_name);
                i++;
            }
        }
        closedir(odir);
        return dirs;
    }

    namespace VFSSerialize
    {
        struct VAssetHeader
        {
            u32 magic_number = VirtualFilesystem::magic_number;
            u32 version = 0;

            u64 size = sizeof(VAssetHeader);
            u64 file_table_offset = size;

            bool write(u64 loc, FILE *f)
            {
                bool res = fseeko(f, loc, SEEK_SET) == 0;

                u32 _magic_number = htole32(magic_number);
                res = res && fwrite(&_magic_number, sizeof(_magic_number), 1, f) == 1;

                u32 _version = htole32(version);
                res = res && fwrite(&_version, sizeof(_version), 1, f) == 1;

                u64 _size = htole64(size);
                res = res && fwrite(&_size, sizeof(_size), 1, f) == 1;

                u64 _file_table_offset = htole64(file_table_offset);
                res = res && fwrite(&_file_table_offset, sizeof(_file_table_offset), 1, f) == 1;

                return res;
            }

            bool read(u64 loc, FILE *f)
            {
                bool res = fseeko(f, loc, SEEK_SET) == 0;

                res = res && fread(&magic_number, sizeof(magic_number), 1, f) == 1;
                magic_number = le32toh(magic_number);

                res = res && fread(&version, sizeof(version), 1, f) == 1;
                version = le32toh(version);

                res = res && fread(&size, sizeof(size), 1, f) == 1;
                size = le64toh(size);

                res = res && fread(&file_table_offset, sizeof(file_table_offset), 1, f) == 1;
                file_table_offset = le64toh(file_table_offset);

                return res;
            }
        };

        struct VFileTable
        {
            u64 size = sizeof(size) + sizeof(entries);
            u32 entries = 0;
            u32 *ids = nullptr;
            u64 *offsets = nullptr;
            u64 *sizes = nullptr;

            bool write(u64 loc, FILE *f)
            {
                bool res = fseeko(f, loc, SEEK_SET) == 0;

                u64 _size = htole64(size);
                res = res && fwrite(&_size, sizeof(_size), 1, f) == 1;

                u32 _entries = htole32(entries);
                res = res && fwrite(&_entries, sizeof(_entries), 1, f) == 1;

                if (entries > 0)
                {
                    u32 *_ids = new u32[entries];
                    u64 *_offsets = new u64[entries];
                    u64 *_sizes = new u64[entries];
                    for (u32 i = 0; i < entries; i++)
                    {
                        _ids[i] = htole32(ids[i]);
                        _offsets[i] = htole64(offsets[i]);
                        _sizes[i] = htole64(sizes[i]);
                    }
                    res = res && fwrite(_ids, sizeof(_ids), 1, f) == 1;
                    res = res && fwrite(_offsets, sizeof(_offsets), 1, f) == 1;
                    res = res && fwrite(_sizes, sizeof(_sizes), 1, f) == 1;

                    delete[] _ids;
                    delete[] _offsets;
                    delete[] _sizes;
                }

                return res;
            }

            bool read(u64 loc, FILE *f)
            {
                bool res = fseeko(f, loc, SEEK_SET) == 0;

                res = res && fread(&size, sizeof(size), 1, f) == 1;
                size = le64toh(size);

                res = res && fread(&entries, sizeof(entries), 1, f) == 1;
                entries = le32toh(entries);

                if (entries > 0)
                {
                    ids = new u32[entries];
                    offsets = new u64[entries];
                    sizes = new u64[entries];

                    res = res && fread(ids, sizeof(ids), 1, f) == 1;
                    res = res && fread(offsets, sizeof(offsets), 1, f) == 1;
                    res = res && fread(sizes, sizeof(sizes), 1, f) == 1;

                    for (u32 i = 0; i < entries; i++)
                    {
                        ids[i] = le32toh(ids[i]);
                        offsets[i] = le64toh(offsets[i]);
                        sizes[i] = le64toh(sizes[i]);
                    }
                }

                return res;
            }

            ~VFileTable()
            {
                if (ids != nullptr)
                {
                    delete[] ids;
                }

                if (offsets != nullptr)
                {
                    delete[] offsets;
                }

                if (sizes != nullptr)
                {
                    delete[] sizes;
                }

                ids = nullptr;
                offsets = nullptr;
                sizes = nullptr;
            }
        };
    } // namespace VFSSerialize

    static bool vfs_write_header(VirtualFilesystem *vfs, FILE *f)
    {
        VFSSerialize::VAssetHeader header;
        header.size = sizeof(header);
        header.version = vfs->version;

        // If the file table offset hasn't been set yet because this is a first time write then just set it to the size of the header since there will be no files or anything
        if (vfs->file_table_offset == 0)
        {
            vfs->file_table_offset = header.size;
        }

        header.file_table_offset = vfs->file_table_offset;
        header.magic_number = vfs->magic_number;

        if (!header.write(0, f))
            return false;
        return true;
    }

    static bool vfs_read_header(VirtualFilesystem *vfs, FILE *f)
    {
        VFSSerialize::VAssetHeader header;
        header.read(0, f);

        if (header.magic_number != VirtualFilesystem::magic_number)
        {
            fprintf(stderr, "Failed to read asset package! File is not a Vultr Asset Package according to the magic number %u in the first 4 bytes of the header. Actual magic number is %u.\n", header.magic_number,
                    VirtualFilesystem::magic_number);
            return false;
        }

        switch (header.version)
        {
            case 0:
            {
                vfs->version = header.version;
                vfs->file_table_offset = header.file_table_offset;
                break;
            }
            default:
            {
                fprintf(stderr, "Failed to read asset package! Version %d is not found!\n", header.version);
                return false;
            }
        }

        return true;
    }

    static bool vfs_write_file_table(VirtualFilesystem *vfs, FILE *f)
    {
        VFSSerialize::VFileTable table;
        table.entries = vfs->file_table.size();
        table.size = sizeof(table.size) + sizeof(table.entries) + table.entries * (sizeof(u32) + sizeof(u64));

        u32 *ids = new u32[table.entries];
        u64 *offsets = new u64[table.entries];
        u64 *sizes = new u64[table.entries];

        u32 i = 0;

        for (auto [id, file] : vfs->file_table)
        {
            ids[i] = file.id;
            offsets[i] = file.offset;
            sizes[i] = file.size;
            i++;
        }

        table.ids = ids;
        table.offsets = offsets;
        table.sizes = sizes;

        return table.write(vfs->file_table_offset, f);
    }

    static bool vfs_read_file_table(VirtualFilesystem *vfs, FILE *f)
    {
        VFSSerialize::VFileTable table;

        if (!table.read(vfs->file_table_offset, f))
            return false;

        vfs->file_table.clear();
        for (u32 i = 0; i < table.entries; i++)
        {
            vfs->file_table[table.ids[i]] = VFile(table.ids[i], table.sizes[i], table.offsets[i]);
        }
        return true;
    }

    VFileHandle::VFileHandle(const char *path)
    {
        fprintf(stderr, "Warning: hashing file path %s at runtime!", path);
        id = crcdetail::compute(path, strlen(path));
    }

    static bool vfs_write_file_entry(VirtualFilesystem *vfs, VFile *file, FILE *f)
    {
        if (fseeko(f, file->offset, SEEK_SET) != 0)
            return false;
        size_t size = sizeof(VFile);
        if (fwrite(file, size, 1, f) != 1)
            return false;
        return true;
    }

    bool vfcreate(VirtualFilesystem *vfs, VFile *file)
    {
        file->offset = vfs->file_table_offset;
        vfs->file_table[file->id] = *file;
        bool res = vfs_write_file_table(vfs, vfs->wfp);
        res = res && vfs_write_file_entry(vfs, file, vfs->wfp);
        fflush(vfs->wfp);
        return res;
    }

    VirtualFilesystem::VirtualFilesystem(const VultrAssetPackage *asset_package, bool write)
    {
        this->asset_package = *asset_package;

        // If one or more of the asset files does not exist, then we need to create them
        if (!fexists(asset_package))
        {
            FILE *f = fopen(asset_package->path, "wb+");
            assert(vfs_write_header(this, f) && "Failed to write header to new asset package!");
            fflush(f);

            assert(vfs_write_file_table(this, f) && "Failed to write file table to new asset package!");

            fclose(f);
            rfp = fopen(asset_package->path, "rb");
        }
        else
        {
            rfp = fopen(this->asset_package.path, "rb");
            assert(vfs_read_header(this, rfp) && vfs_read_file_table(this, rfp) && "Failed to read asset package!");
        }

        if (write)
        {
            wfp = fopen(this->asset_package.path, "rb+");
        }
    }

    VirtualFilesystem::~VirtualFilesystem()
    {
        if (rfp != nullptr)
        {
            fclose(rfp);
            rfp = nullptr;
        }

        if (wfp != nullptr)
        {
            fclose(wfp);
            wfp = nullptr;
        }
    }

} // namespace Vultr
