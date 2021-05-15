#include <core/system_providers/input_system_provider.h>
#include <core/systems/input_system.h>

namespace Vultr::InputSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }

    bool key_down(const char key)
    {
        int state = glfwGetKey(get_provider().window, std::toupper(key));
        return state == GLFW_PRESS;
    }

    void add_scroll_input(glm::vec2 input)
    {
        get_provider().scroll_queue.push(input);
    }

} // namespace Vultr::InputSystem
