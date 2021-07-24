#include <gtest/gtest.h>
#include "gui/basic_rendering_test.h"
#include <rendering/types/material.h>

int main(int argc, char **argv)
{
    // basic_rendering_test();
    // testing::InitGoogleTest(&argc, argv);
    // return RUN_ALL_TESTS();
    using namespace Vultr;
    Material material = {};
    fprintf(stdout, "%lu", sizeof(material));
}
