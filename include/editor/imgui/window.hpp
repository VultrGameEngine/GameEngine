#pragma once
#include <string>
#include "element.hpp"
#include <vector>

namespace Editor
{
    class Window
    {
    public:
        virtual void Render() = 0;
    };
}; // namespace Editor