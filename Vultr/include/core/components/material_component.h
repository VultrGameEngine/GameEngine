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

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct MaterialComponent
{
    typedef struct
    {
        Vultr::File path;
        u16 slot;
        std::string name;
    } TexturePair;

    Vultr::File shader_source;
    std::vector<TexturePair> textures;
    std::unordered_map<std::string, glm::vec3> vec3s;
    std::unordered_map<std::string, glm::vec4> vec4s;
    std::unordered_map<std::string, Color> colors;
    std::unordered_map<std::string, s32> ints;
    std::unordered_map<std::string, f32> floats;

    static MaterialComponent Create()
    {
        auto component = MaterialComponent();
        component.shader_source.extension = SHADER_FILE_EXTENSIONS;
        return component;
    }

    std::vector<std::string> get_paths()
    {
        auto vec = std::vector<std::string>();
        vec.reserve(textures.size());
        for (auto pair : textures)
        {
            vec.push_back(pair.path.GetPath().string());
        }
        return vec;
    }
};
