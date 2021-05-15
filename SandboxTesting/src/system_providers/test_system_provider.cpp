#include <system_providers/test_system_provider.h>

namespace TestSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }
} // namespace TestSystem
