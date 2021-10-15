#include <gtest/gtest.h>
#include <rendering/types/material.h>
#include <sys/resource.h>

int main(int argc, char **argv)
{
    struct rlimit core_limits;
    core_limits.rlim_cur = core_limits.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &core_limits);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    using namespace Vultr;
    Material material = {};
    fprintf(stdout, "%lu", sizeof(material));
}
