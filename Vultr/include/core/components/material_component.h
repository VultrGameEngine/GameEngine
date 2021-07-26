#pragma once
#include <glm/glm.hpp>
#include <helpers/file.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <types/types.hpp>
#include <vector>
#include <core/models/color.h>
#include <rendering/types/material.h>

namespace Vultr
{
    struct DirectionalLight
    {
        Vec3 direction;
        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;
    };

    struct MaterialComponent : Material
    {

        static MaterialComponent Create()
        {
            auto component = MaterialComponent();
            return component;
        }
    };
} // namespace Vultr
