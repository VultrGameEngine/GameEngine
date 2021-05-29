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

namespace Vultr
{
    struct DirectionalLight
    {
        Vec3 direction;
        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;
    };

    struct MaterialComponent
    {
        typedef struct
        {
            Vultr::TextureSource file;
            u16 slot;
            std::string name;
        } TexturePair;

        Vultr::ShaderSource shader_source;
        std::vector<TexturePair> textures;
        std::unordered_map<std::string, Vec3> vec3s;
        std::unordered_map<std::string, Vec4> vec4s;
        std::unordered_map<std::string, Color> colors;
        std::unordered_map<std::string, s32> ints;
        std::unordered_map<std::string, f32> floats;

        static MaterialComponent Create()
        {
            auto component = MaterialComponent();
            return component;
        }

        std::vector<Vultr::TextureSource> get_paths()
        {
            auto vec = std::vector<Vultr::TextureSource>();
            vec.reserve(textures.size());
            for (auto pair : textures)
            {
                vec.push_back(pair.file);
            }
            return vec;
        }
    };
} // namespace Vultr
