#pragma once

struct UpdateTick;
namespace Vultr::InputSystem
{

    void register_system();
    void init(GLFWwindow *window);
    void update(const UpdateTick &tick);
    void on_mouse_input(GLFWwindow *window, int button, int action, int mods);
    void on_scroll(GLFWwindow *window, double xoffset, double yoffset);

} // namespace Vultr::InputSystem
