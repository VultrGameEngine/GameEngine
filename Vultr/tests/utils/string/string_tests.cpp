#include <gtest/gtest.h>
#define private public
#define protected public

#include <utils/string/string.h>

using namespace Vultr;
TEST(String_Utils, Str)
{
    char *string = str(100);

    strcat(string, "Hello");
    ASSERT_EQ(strlen(string), strlen("Hello"));
    ASSERT_EQ(strcmp(string, "Hello"), 0);

    strcpy(string, "Hello \0");
    strcat(string, "My Baby\0");
    ASSERT_EQ(strlen(string), strlen("Hello My Baby"));
    ASSERT_EQ(strcmp(string, "Hello My Baby"), 0);
    free(string);

    const char *string_literal = "Hello my baby, hello my honey, hello my ragtime gal";
    string = strn(string_literal, 5);
    ASSERT_EQ(strlen(string), 5);
    ASSERT_EQ(strcmp(string, "Hello"), 0);
    ASSERT_EQ(string[5], '\0');
    free(string);

    string = str(string_literal);
    ASSERT_EQ(strcmp(string, string_literal), 0);
    free(string);

    char *empty_str = str("");
    ASSERT_EQ(strcmp(empty_str, ""), 0);
    free(empty_str);
}

TEST(String_Utils, StrEqual)
{
    const char *string_literal = "Hello my baby, hello my honey, hello my ragtime gal";
    char *string = str(string_literal);

    ASSERT_TRUE(strequal(string, string_literal));
    free(string);

    string = str("");
    ASSERT_TRUE(strequal(string, ""));
}

TEST(String_Utils, Restr)
{
    const char *str1 = "Hello";
    char *string = str(str1);

    const char *str2 = " My ";
    string = restr(string, strlen(str1) + strlen(str2));
    strcat(string, str2);
    ASSERT_TRUE(strequal(string, "Hello My "));
    free(string);

    string = str("");
    restr(string, strlen(str2));
    strcat(string, str2);
    ASSERT_TRUE(strequal(string, str2));
    free(string);
}

TEST(String_Utils, Strappend)
{
    const char *str1 = "Hello";
    char *string = str(str1);

    const char *str2 = " My ";
    strappend(string, str2);
    ASSERT_TRUE(strequal(string, "Hello My "));
    free(string);

    string = str("");
    strappend(string, str1);
    strappend(string, str2);
    ASSERT_TRUE(strequal(string, "Hello My "));
    free(string);
}
