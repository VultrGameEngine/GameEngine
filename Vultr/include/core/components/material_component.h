#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cereal/types/vector.hpp>

struct MaterialComponent
{
    std::string shader_path;
    std::unordered_map<std::string, GLenum> textures;
    std::unordered_map<std::string, glm::vec3> vec3s;
    std::unordered_map<std::string, int> ints;
    std::unordered_map<std::string, float> floats;

    static MaterialComponent Create()
    {
        return MaterialComponent();
    }

    char *identifier = nullptr;

    template <class Archive> void serialize(Archive &ar)
    {
        ar(shader_path, textures);
    }
};
