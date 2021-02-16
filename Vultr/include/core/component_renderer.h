#pragma once
#include <imgui/imgui.h>
#include "core_components.h"

typedef void (*ComponentRender)(Entity);

void RenderTransformComponent(Entity entity);

void RenderMaterialComponent(Entity entity);

void RenderLightComponent(Entity entity);

void RenderControllerComponent(Entity entity);

void RenderStaticMeshComponent(Entity entity);

void RenderCameraComponent(Entity entity);

void RenderSkyboxComponent(Entity entity);

void RenderForwardMaterialComponent(Entity entity);

void RenderDefaultComponent(Entity entity);
