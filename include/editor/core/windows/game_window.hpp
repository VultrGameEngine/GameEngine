#pragma once
#include "../../imgui/window.hpp"

namespace Editor
{
    class GameWindow : public Window
    {
    public:
        void Render() override;
    };
}; // namespace Editor