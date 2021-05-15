#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <types/types.hpp>

struct Color
{
    Color(glm::vec4 p_value) : value(p_value)
    {
    }
    glm::vec4 value;
};

struct MaterialComponent
{
    typedef struct
    {
        const char *path;
        u16 slot;
    } TexturePair;

    const char *shader_path;
    std::vector<TexturePair> textures;
    std::unordered_map<const char *, glm::vec3> vec3s;
    std::unordered_map<const char *, glm::vec4> vec4s;
    std::unordered_map<const char *, Color> colors;
    std::unordered_map<const char *, int> ints;
    std::unordered_map<const char *, float> floats;

    static MaterialComponent Create()
    {
        return MaterialComponent();
    }

    const char *identifier = nullptr;
};
