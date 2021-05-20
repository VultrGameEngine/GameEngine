#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <types/types.hpp>
#include <vector>
#include <json/json.hpp>
#include <json/glm_serializer.hpp>

struct Color
{
    Color() : value(glm::vec4(0, 0, 0, 1))
    {
    }
    Color(glm::vec4 p_value) : value(p_value)
    {
    }
    glm::vec4 value;
};

struct MaterialComponent
{
    typedef struct
    {
        std::string path;
        u16 slot;
    } TexturePair;

    std::string shader_path;
    std::vector<TexturePair> textures;
    std::unordered_map<std::string, glm::vec3> vec3s;
    std::unordered_map<std::string, glm::vec4> vec4s;
    std::unordered_map<std::string, Color> colors;
    std::unordered_map<std::string, int> ints;
    std::unordered_map<std::string, float> floats;

    static MaterialComponent Create()
    {
        return MaterialComponent();
    }

    const char *identifier = nullptr;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MaterialComponent::TexturePair, path, slot);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Color, value);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MaterialComponent, shader_path, textures, vec3s, vec4s, colors, ints, floats);
