#include <core/systems/rmlui_system.h>

namespace Vultr
{
void RmlUiSystem::RegisterSystem()
{
    Signature signature;
    World::RegisterSystem<RmlUiSystemProvider>(signature);
}

void RmlUiSystem::Update()
{
    RmlUiSystemProvider &provider = GetProvider();
}
} // namespace Vultr
