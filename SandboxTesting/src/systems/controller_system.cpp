#include <systems/controller_system.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

glm::quat safeQuatLookAt(glm::vec3 const &lookFrom, glm::vec3 const &lookTo,
                         glm::vec3 const &up)
{
    glm::vec3 direction = lookTo - lookFrom;
    float directionLength = glm::length(direction);

    // Check if the direction is valid; Also deals with NaN
    if (!(directionLength > 0.0001))
        return glm::quat(1, 0, 0, 0); // Just return identity

    // Normalize direction
    direction /= directionLength;

    return glm::quatLookAt(direction, up);
}

void ControllerSystem::OnScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    ControllerSystemProvider &provider = GetProvider();
    provider.radius -= yoffset * 20;
}

void ControllerSystem::RegisterSystem()
{
    Signature signature;
    signature.set(World::GetComponentType<ControllerComponent>(), true);
    World::RegisterSystem<ControllerSystemProvider>(signature);
}

void ControllerSystem::OnCreateEntity(Entity entity)
{
    ControllerSystemProvider &provider = GetProvider();
    provider.controller = entity;
}

void ControllerSystem::OnDestroyEntity(Entity entity)
{
    ControllerSystemProvider &provider = GetProvider();
    for (Entity entity : provider.entities)
    {
        provider.controller = entity;
        return;
    }
    provider.controller = Entity(-1);
}

void ControllerSystem::Init(const glm::vec3 &center, GLFWwindow *window)
{
    ControllerSystemProvider &provider = GetProvider();
    provider.center = TransformComponent::Create(center);
    provider.window = window;
    glfwSetScrollCallback(window, ControllerSystem::OnScroll);
}

void ControllerSystem::Update(float delta_time)
{
    ControllerSystemProvider &provider = GetProvider();
    if (provider.controller.id == -1 || provider.window == nullptr)
        return;
    if (!Vultr::ControllerSystemProvider::Get()->m_focused)
        return;

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glm::vec2 dimensions = glm::vec2(mode->width, mode->height);
    double xpos, ypos;
    glfwGetCursorPos(provider.window, &xpos, &ypos);
    glfwSetCursorPos(provider.window, dimensions.x / 2, dimensions.y / 2);

    auto &controller_component =
        provider.controller.GetComponent<ControllerComponent>();

    auto &transform_component =
        provider.controller.GetComponent<TransformComponent>();

    glm::quat rotation_horiz = glm::angleAxis(
        controller_component.sens * delta_time * float(dimensions.x / 2 - xpos),
        glm::vec3(0, 1, 0));
    glm::quat rotation_vert = glm::angleAxis(controller_component.sens * delta_time *
                                                 float(dimensions.y / 2 - ypos),
                                             provider.center.Right());
    provider.center.rotation =
        rotation_horiz * rotation_vert * provider.center.rotation;

    glm::vec3 position =
        provider.center.rotation * glm::vec3(0, 0, -provider.radius) +
        provider.center.position;
    transform_component.position = position;
    transform_component.rotation =
        safeQuatLookAt(transform_component.position, provider.center.position,
                       transform_component.Up());
}
