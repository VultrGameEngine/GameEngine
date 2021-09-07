#include <gtest/gtest.h>
#define private public
#define protected public

#include <stdio.h>
#include <filesystem/file.h>

static const char *file_path = "test_file.txt";
static const char *file_extension = ".txt";
static const char *file_data = "string";

using namespace Vultr;
class FileTests : public testing::Test
{
  protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite()
    {
        FILE *f = fopen(file_path, "w+");
        fputs(file_data, f);
        fclose(f);
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite()
    {
        remove(file_path);
    }
};

TEST_F(FileTests, FBasename)
{
    size_t len;
    GenericFile file = GenericFile(file_path);
    EXPECT_STRCASEEQ(fbasename(&file, &len), file_path);
    EXPECT_EQ(len, strlen(file_path));

    GenericFile local_file = GenericFile("./test_file.cpp");
    EXPECT_STRCASEEQ(fbasename(&local_file, &len), "test_file.cpp");
    EXPECT_EQ(len, strlen("test_file.cpp"));

    GenericFile windows_file = GenericFile("C:\\Windows Path\\test_file.cpp");
    EXPECT_STRCASEEQ(windows_file.path, "C:/Windows Path/test_file.cpp");
    EXPECT_STRCASEEQ(fbasename(&windows_file, &len), "test_file.cpp");
    EXPECT_EQ(len, strlen("test_file.cpp"));
}

TEST_F(FileTests, FExtension)
{
    GenericFile file = GenericFile(file_path);
    EXPECT_TRUE(strequal(fextension(&file), file_extension));

    file = GenericFile("no_extension");
    EXPECT_EQ(fextension(&file), nullptr);
}

TEST_F(FileTests, FRename)
{
    GenericFile file = GenericFile(file_path);
    file_path = "new_test_file.text";
    file_extension = ".text";
    frename(&file, file_path);

    size_t len;

    EXPECT_STRCASEEQ(fextension(&file), file_extension);
    EXPECT_STRCASEEQ(fbasename(&file, &len), file_path);
    EXPECT_EQ(len, strlen(file_path));

    FILE *f = fopen(file_path, "r");
#define READ_BUFFER_SIZE 32768
    char buffer[READ_BUFFER_SIZE];

    char *res_str = str("");

    while (!feof(f))
    {
        size_t bytes = fread(buffer, 1, sizeof(buffer), f);

        if (bytes)
        {
            res_str = strappend(res_str, buffer);
        }
    }
    EXPECT_TRUE(strequal(res_str, file_data));

    fclose(f);
}

TEST_F(FileTests, FCopy)
{
    const char *copy_file_path = "copy_file.txt";
    GenericFile file = GenericFile(file_path);

    bool res = fcopy(&file, copy_file_path);
    ASSERT_TRUE(res);

    ASSERT_TRUE(strequal(file.path, copy_file_path));

    FILE *f = fopen(copy_file_path, "r");
#define READ_BUFFER_SIZE 32768
    char buffer[READ_BUFFER_SIZE];

    char *res_str = str("");

    while (!feof(f))
    {
        size_t bytes = fread(buffer, 1, sizeof(buffer), f);

        if (bytes)
        {
            res_str = strappend(res_str, buffer);
        }
    }
    EXPECT_TRUE(strequal(res_str, file_data));

    fclose(f);
}

TEST_F(FileTests, FRemove_FExists)
{
    const char *copy_file_path = "copy_file.txt";
    GenericFile file = GenericFile(copy_file_path);

    bool removal_successful = fremove(&file);
    ASSERT_TRUE(removal_successful);

    bool copy_exists = fexists(&file);
    ASSERT_FALSE(copy_exists);

    GenericFile orig = GenericFile(file_path);

    bool orig_exists = fexists(&orig);
    ASSERT_TRUE(orig_exists);
}
