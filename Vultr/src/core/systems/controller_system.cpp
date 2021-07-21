#include <core/systems/controller_system.h>
#include <core/system_providers/controller_system_provider.h>
#include <core/system_providers/camera_system_provider.h>
#include <core/system_providers/input_system_provider.h>
#include <core/system_providers/render_system_provider.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <engine.hpp>

namespace Vultr::ControllerSystem
{

    void init(Engine *e, GLFWwindow *window)
    {
        get_provider(e).window = window;
    }

    void update(Engine *e, float delta_time)
    {
        auto &transform_component = CameraSystem::get_provider(e).scene_camera.transform_component;

        auto &controller_component = CameraSystem::get_provider(e).scene_camera.controller_component;

        if (!InputSystem::mouse_is_on_screen(e, InputSystem::get_provider(e).scene_mouse_pos))
            return;

        ControllerSystem::Component &provider = get_provider(e);
        if (!provider.focused)
            return;
        if (!glfwGetMouseButton(provider.window, GLFW_MOUSE_BUTTON_RIGHT))
        {
            glfwSetInputMode(provider.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            return;
        }
        Vec2 dimensions = RenderSystem::get_dimensions(e, SCENE);

        double xpos, ypos;
        glfwGetCursorPos(provider.window, &xpos, &ypos);

        glfwSetCursorPos(provider.window, dimensions.x / 2, dimensions.y / 2);

        Quat rotation_horiz = glm::angleAxis(controller_component.sens * delta_time * float(dimensions.x / 2 - xpos), Vec3(0, 1, 0));
        Quat rotation_vert = glm::angleAxis(controller_component.sens * delta_time * float(dimensions.y / 2 - ypos), transform_component.Right());
        transform_component.rotation = rotation_horiz * rotation_vert * transform_component.rotation;

        Vec3 speed = Vec3(3, 3, 3);

        // Move forward
        if (glfwGetKey(provider.window, GLFW_KEY_W) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Forward() * delta_time * speed;
        }
        // Move backward
        if (glfwGetKey(provider.window, GLFW_KEY_S) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Forward() * delta_time * speed;
        }
        // Strafe right
        if (glfwGetKey(provider.window, GLFW_KEY_D) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Right() * delta_time * speed;
        }
        // Strafe left
        if (glfwGetKey(provider.window, GLFW_KEY_A) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Right() * delta_time * speed;
        }
        // Strafe up
        if (glfwGetKey(provider.window, GLFW_KEY_E) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Up() * delta_time * speed;
        }
        // Strafe down
        if (glfwGetKey(provider.window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Up() * delta_time * speed;
        }
    }
    void window_focus_callback(GLFWwindow *window, int focused)
    {
        Engine *e = get_engine(window);
        Component &provider = get_provider(e);
        provider.focused = focused;
        if (provider.focused)
        {
            glfwSetCursorPos(provider.window, RenderSystem::get_dimensions(e, GAME).x / 2, RenderSystem::get_dimensions(e, GAME).y / 2);
        }
    }

    void register_system(Engine *e)
    {
        Signature signature;
        signature.set(get_component_type<ControllerComponent>(e), true);
        signature.set(get_component_type<TransformComponent>(e), true);
        register_global_system<Component>(e, signature, nullptr, nullptr);
    }
} // namespace Vultr::ControllerSystem
