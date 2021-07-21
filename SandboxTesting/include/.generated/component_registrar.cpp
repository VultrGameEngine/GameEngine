// TODO generate this file
#include <component_registrar.h>
#include "../components/generated/test_component.generated.h"

using namespace Vultr;
void register_components(Vultr::Engine *e)
{
    register_component<TestComponent>(e);
}
