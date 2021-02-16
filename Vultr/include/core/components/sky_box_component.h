#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

struct SkyBoxComponent
{

    static std::shared_ptr<SkyBoxComponent> Create(
        const std::string &identifier = "default",
        const std::string &front = "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                   "res/textures/skybox/front.jpg",
        const std::string &back = "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                  "res/textures/skybox/back.jpg",
        const std::string &top = "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                 "res/textures/skybox/top.jpg",
        const std::string &bottom = "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                    "res/textures/skybox/bottom.jpg",
        const std::string &left = "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                  "res/textures/skybox/left.jpg",
        const std::string &right = "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                   "res/textures/skybox/right.jpg")
    {
        std::shared_ptr<SkyBoxComponent> component =
            std::make_shared<SkyBoxComponent>();
        component->identifier = identifier;
        component->front = front;
        component->back = back;
        component->top = top;
        component->bottom = bottom;
        component->left = left;
        component->right = right;
        return component;
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
    template <class Archive> void serialize(Archive &ar)
    {
        ar(identifier, front, back, top, bottom, left, right);
    }

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
