#include <gtest/gtest.h>
#define private public
#define protected public

#include <utils/string/string.h>

using namespace Vultr;
TEST(String_Utils, String)
{
    char *string = str(10);
    ASSERT_EQ(strlen(string), 10);
}
