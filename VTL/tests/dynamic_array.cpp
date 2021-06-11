#include <gtest/gtest.h>
#define private public
#define protected public
#include <dynamic_array.h>

using namespace vtl;
TEST(ArrayList, EmptyInitialize)
{
    DynamicArray<const char *> string_list = new_dynamic_array<const char *>();
    ASSERT_EQ(string_list.size, 0);
    ASSERT_EQ(string_list.used, 0);
    ASSERT_EQ(string_list.internal_array, nullptr);
}

TEST(ArrayList, ArrayInitialize)
{
    const char *string_array[] = {"hello", "world"};
    DynamicArray<const char *> string_list = new_dynamic_array(string_array, 2);
    ASSERT_EQ(string_list.size, 2);
    ASSERT_EQ(string_list.used, 2);
    ASSERT_NE(string_list.internal_array, nullptr);
}

TEST(ArrayList, Operator)
{
    const char *string_array[] = {"hello"};
    DynamicArray<const char *> string_list = new_dynamic_array(string_array, 1);
    ASSERT_EQ("hello", string_list[0]);
    string_list[0] = "world";
    ASSERT_EQ("world", string_list[0]);
    ASSERT_DEATH(string_list[1] = "ur mother", "Index out of bounds");
}

TEST(ArrayList, PushBack)
{
    const char *string_array[] = {"hello"};
    DynamicArray<const char *> string_list = new_dynamic_array(string_array, 1);

    dynamic_array_push_back(string_list, "world");
    ASSERT_EQ(string_list[1], "world");
    ASSERT_EQ(string_list.size, 2);
    ASSERT_EQ(string_list.used, 2);
}

TEST(ArrayList, Insert)
{
    const char *string_array[] = {"big", "big", "chungus", "big", "chungus"};
    DynamicArray<const char *> string_list = new_dynamic_array(string_array, 5);

    // Insert middle
    dynamic_array_insert(string_list, 1, "joe");
    ASSERT_EQ(string_list[0], "big");
    ASSERT_EQ(string_list[1], "joe");
    ASSERT_EQ(string_list[2], "big");
    ASSERT_EQ(string_list[3], "chungus");
    ASSERT_EQ(string_list[4], "big");
    ASSERT_EQ(string_list[5], "chungus");
    ASSERT_EQ(string_list.size, 6);
    ASSERT_EQ(string_list.used, 6);

    // Insert beginning
    dynamic_array_insert(string_list, 0, "mama");
    ASSERT_EQ(string_list[0], "mama");
    ASSERT_EQ(string_list[1], "big");
    ASSERT_EQ(string_list[2], "joe");
    ASSERT_EQ(string_list[3], "big");
    ASSERT_EQ(string_list[4], "chungus");
    ASSERT_EQ(string_list[5], "big");
    ASSERT_EQ(string_list[6], "chungus");
    ASSERT_EQ(string_list.size, 7);
    ASSERT_EQ(string_list.used, 7);

    // Insert end / push_back
    dynamic_array_insert(string_list, 7, "urmother");
    ASSERT_EQ(string_list[0], "mama");
    ASSERT_EQ(string_list[1], "big");
    ASSERT_EQ(string_list[2], "joe");
    ASSERT_EQ(string_list[3], "big");
    ASSERT_EQ(string_list[4], "chungus");
    ASSERT_EQ(string_list[5], "big");
    ASSERT_EQ(string_list[6], "chungus");
    ASSERT_EQ(string_list[7], "urmother");
    ASSERT_EQ(string_list.size, 8);
    ASSERT_EQ(string_list.used, 8);
}

TEST(ArrayList, Delete)
{
    const char *string_array[] = {"big", "big", "chungus", "big", "chungus"};
    DynamicArray<const char *> string_list = new_dynamic_array(string_array, 5);

    dynamic_array_delete_element(string_list, 1);
    ASSERT_EQ(string_list[0], "big");
    ASSERT_EQ(string_list[1], "chungus");
    ASSERT_EQ(string_list[2], "big");
    ASSERT_EQ(string_list[3], "chungus");
    ASSERT_EQ(string_list.size, 4);
    ASSERT_EQ(string_list.used, 4);
    dynamic_array_delete_element(string_list, 3);
    ASSERT_EQ(string_list[0], "big");
    ASSERT_EQ(string_list[1], "chungus");
    ASSERT_EQ(string_list[2], "big");
    ASSERT_EQ(string_list.size, 3);
    ASSERT_EQ(string_list.used, 3);
    dynamic_array_delete_element(string_list, 0);
    ASSERT_EQ(string_list[0], "chungus");
    ASSERT_EQ(string_list[1], "big");
    ASSERT_EQ(string_list.size, 2);
    ASSERT_EQ(string_list.used, 2);
    dynamic_array_delete_element(string_list, 0);
    dynamic_array_delete_element(string_list, 0);
    ASSERT_EQ(string_list.size, 0);
    ASSERT_EQ(string_list.used, 0);
}

TEST(ArrayList, Iterator)
{
    const char *string_array[] = {"big", "big", "chungus", "big", "chungus"};
    DynamicArray<const char *> string_list = new_dynamic_array(string_array, 5);

    uint i = 0;
    for (const char *val : string_list)
    {
        EXPECT_STREQ(val, string_array[i]);
        i++;
    }
}
