#pragma once
#include "../../imgui/window.hpp"

namespace Editor
{
    class SceneWindow : public Window
    {
    public:
        void Render() override;
    };
}; // namespace Editor