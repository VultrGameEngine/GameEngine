#pragma once
class Entity;
typedef void (*ComponentConstructor)(Entity);

void ConstructForwardMaterialComponent(Entity entity);
