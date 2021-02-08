#pragma once
#include "imgui/window.hpp"
#include <vector>
// #include "../core/components/camera_component.h"
// #include "../core/components/transform_component.h"

namespace Editor
{
    class Editor
    {
    public:
        Editor();
        ~Editor();
        static Editor *Get()
        {
            static Editor *instance;
            if (instance == nullptr)
            {
                instance = new Editor();
            }
            return instance;
        }
        static unsigned int GetDockSpace();
        static void Render();

    private:
        std::vector<Window *> windows;
        unsigned int dockspace = 1;
    };
} // namespace Editor
