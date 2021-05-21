// TODO generate this file
#include <component_registrar.h>
#include "../components/generated/test_component.generated.h"

using namespace Vultr;
void register_components()
{
    engine_register_default_components(engine_global());
    register_component<TestComponent>();
    engine_init_default_systems(engine_global());
}
