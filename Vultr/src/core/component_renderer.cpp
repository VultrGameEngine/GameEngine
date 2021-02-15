#include <core/component_renderer.h>
#include <ecs/world/world.hpp>

using namespace Brick3D;
void RenderVec3(const std::string &name, glm::vec3 &value)
{
    ImGui::DragFloat((name + "X").c_str(), &value.x, 0.02f);
    ImGui::DragFloat((name + "Y").c_str(), &value.y, 0.02f);
    ImGui::DragFloat((name + "Z").c_str(), &value.z, 0.02f);
}

void RenderQuat(const std::string &name, glm::quat &value)
{
    ImGui::DragFloat((name + "X").c_str(), &value.x, 0.02f);
    ImGui::DragFloat((name + "Y").c_str(), &value.y, 0.02f);
    ImGui::DragFloat((name + "Z").c_str(), &value.z, 0.02f);
    ImGui::DragFloat((name + "W").c_str(), &value.w, 0.02f);
}

void RenderTransformComponent(Entity entity)
{
    TransformComponent *component = entity.GetComponentUnsafe<TransformComponent>();
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("Transform Component"))
    {
        ImGui::Text("Position");
        RenderVec3("Position", component->position);
        ImGui::Text("Rotation");
        RenderQuat("Rotation", component->rotation);
        ImGui::Text("Scale");
        RenderVec3("Scale", component->scale);
    }
}

void RenderMaterialComponent(Entity entity)
{
    MaterialComponent *component = entity.GetComponentUnsafe<MaterialComponent>();
    if (component == nullptr)
        return;
    ImGui::Text("Here is a material component");
}

void RenderLightComponent(Entity entity)
{
}

void RenderControllerComponent(Entity entity)
{
}

void RenderStaticMeshComponent(Entity entity)
{
    StaticMeshComponent *component =
        entity.GetComponentUnsafe<StaticMeshComponent>();
    if (component == nullptr)
        return;
    ImGui::Text("Here is a controller component");
}

void RenderCameraComponent(Entity entity)
{
    CameraComponent *component = entity.GetComponentUnsafe<CameraComponent>();
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("Camera Component"))
    {
        ImGui::Checkbox("Enabled", &component->enabled);
        ImGui::DragFloat("FOV", &component->fov);
        ImGui::DragFloat("ZNear", &component->znear);
        ImGui::DragFloat("ZFar", &component->zfar);
    }
}

void RenderSkyboxComponent(Entity entity)
{
    SkyBoxComponent *component = entity.GetComponentUnsafe<SkyBoxComponent>();
    if (component == nullptr)
        return;
}

void RenderDefaultComponent(Entity entity)
{
    ImGui::Text("Default component render function");
}
