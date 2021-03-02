#pragma once
#include <ecs/entity/entity.hpp>
#include <core/core_components.h>

typedef void (*ComponentConstructor)(Entity);

void ConstructForwardMaterialComponent(Entity entity);
