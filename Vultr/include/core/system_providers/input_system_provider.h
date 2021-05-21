#pragma once
#include <ecs/system/system_provider.hpp>
#include <core/systems/input_system.h>
#include <engine.hpp>
#include <GLFW/glfw3.h>
#include <queue>

namespace Vultr
{

    namespace InputSystem
    {
        struct Component : public SystemProvider
        {
            std::queue<glm::vec2> scroll_queue;
            GLFWwindow *window;
            glm::vec2 mouse_pos = glm::vec2(0, 0);
            glm::vec2 scene_mouse_pos = glm::vec2(0, 0);
            glm::vec2 scroll_amount = glm::vec2(0, 0);
            bool mouse_down = false;
            std::unordered_map<const char *, MouseClickEvent> mouse_click_events;
        };

        Component &get_provider();

        bool key_down(const char key);
        void add_scroll_input(glm::vec2 input);
    } // namespace InputSystem

    template <>
    inline const char *get_struct_name<InputSystem::Component>()
    {
        return "InputSystem";
    }

} // namespace Vultr
