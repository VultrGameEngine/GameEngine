#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct SkyBoxComponent
{

    static SkyBoxComponent &Create(const std::string &identifier,
                                   const std::string &front, const std::string &back,
                                   const std::string &top, const std::string &bottom,
                                   const std::string &left, const std::string &right)
    {
        SkyBoxComponent *component = new SkyBoxComponent{
            .identifier = identifier,
            .front = front,
            .back = back,
            .top = top,
            .bottom = bottom,
            .left = left,
            .right = right,
        };
        return *component;
    }
    unsigned int vao = 0;
    unsigned int vbo = 0;
    std::vector<glm::vec3> vertices;
    std::string identifier;
    std::string front;
    std::string back;
    std::string top;
    std::string bottom;
    std::string left;
    std::string right;

    std::vector<std::string> GetPaths()
    {
        std::vector<std::string> paths = {right, left, top, bottom, front, back};
        return paths;
    }

    std::vector<glm::vec3> GetVertices()
    {
        if (vertices.size() == 0)
            vertices = {
                // Front
                glm::vec3(1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(-1.0f, 1.0f, -1.0f),
                glm::vec3(-1.0f, 1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, -1.0f, -1.0f),

                // Left
                glm::vec3(-1.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, 1.0f, -1.0f),
                glm::vec3(-1.0f, 1.0f, -1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, 1.0f),

                // Right
                glm::vec3(1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, -1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.0f, -1.0f, -1.0f),

                // Back
                glm::vec3(-1.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, 1.0f),

                // Top
                glm::vec3(-1.0f, 1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f, -1.0f),

                // Bottom
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, -1.0f),
                glm::vec3(1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, 1.0f),
            };

        return vertices;
    }
};
