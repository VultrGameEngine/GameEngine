#include <core/component_constructor.h>

void ConstructForwardMaterialComponent(Entity entity)
{
    entity.AddComponent(ForwardMaterial::Create());
}
