#include <gtest/gtest.h>
#define private public
#define protected public

#include <filesystem/directory.h>
#include <utils/string/string.h>

static const char *dir_path = "res";

using namespace Vultr;
class DirectoryTests : public testing::Test
{
  protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite()
    {
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite()
    {
    }
};

TEST_F(DirectoryTests, Directory)
{
    auto dir = Directory(dir_path);
    EXPECT_STRCASEEQ(dir.path, "res/");
    dir = Directory("res/");
    EXPECT_STRCASEEQ(dir.path, "res/");
    dir = Directory("C:\\Windows Path\\");
    EXPECT_STRCASEEQ(dir.path, "C:/Windows Path/");
}

TEST_F(DirectoryTests, Dirbasename)
{
    size_t len;
    auto dir = Directory("./res/");
    EXPECT_STRCASEEQ(dir.path, "./res/");

    EXPECT_STRCASEEQ(dirbasename(&dir, &len), "res/");
    EXPECT_EQ(len, strlen("res"));

    dir = Directory("/res/otherpath/");
    EXPECT_STRCASEEQ(dirbasename(&dir, &len), "otherpath/");
    EXPECT_EQ(len, strlen("otherpath"));

    dir = Directory("/res/otherpath");
    EXPECT_STRCASEEQ(dirbasename(&dir, &len), "otherpath/");
    EXPECT_EQ(len, strlen("otherpath"));

    dir = Directory("/");
    EXPECT_STRCASEEQ(dirbasename(&dir, &len), "/");
    EXPECT_EQ(len, strlen("/"));

    dir = Directory("./");
    EXPECT_STRCASEEQ(dirbasename(&dir, &len), "./");
    EXPECT_EQ(len, strlen("./"));

    dir = Directory("//");
    EXPECT_STRCASEEQ(dirbasename(&dir, &len), "/");
    EXPECT_EQ(len, 0);

    dir = Directory(".//");
    EXPECT_STRCASEEQ(dirbasename(&dir, &len), "/");
    EXPECT_EQ(len, 0);

    dir = Directory("res/");
    EXPECT_STRCASEEQ(dir.path, "res/");
}

TEST_F(DirectoryTests, Dirparent)
{
    auto dir = Directory(dir_path);
    Directory parent;
    dirparent(&dir, &parent);
    EXPECT_STRCASEEQ(parent.path, "./");

    dir = Directory("/usr/bin");
    dirparent(&dir, &parent);
    EXPECT_STRCASEEQ(parent.path, "/usr/");
}

TEST_F(DirectoryTests, Dirmake_Direxists_Dirremove)
{
    const char *second_dir_path = "testdir/recursivedir/";
    Directory dir;
    ASSERT_TRUE(dirmake(second_dir_path, &dir));
    ASSERT_TRUE(direxists(&dir));

    ASSERT_TRUE(dirremove(&dir));
    ASSERT_FALSE(direxists(&dir));

    dir = Directory("testdir");
    ASSERT_TRUE(dirremove(&dir));
    ASSERT_FALSE(direxists(&dir));
}
