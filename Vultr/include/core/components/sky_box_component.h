#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <json/json.hpp>
#include <json/glm_serializer.hpp>

struct SkyBoxComponent
{

    static SkyBoxComponent Create(const char *identifier = "default", const char *front = "textures/skybox/front.jpg", const char *back = "textures/skybox/back.jpg",
                                  const char *top = "textures/skybox/top.jpg", const char *bottom = "textures/skybox/bottom.jpg", const char *left = "textures/skybox/left.jpg",
                                  const char *right = "textures/skybox/right.jpg")
    {
        SkyBoxComponent component = SkyBoxComponent();
        component.identifier = identifier;
        component.front = front;
        component.back = back;
        component.top = top;
        component.bottom = bottom;
        component.left = left;
        component.right = right;
        return component;
    }

    std::string identifier;
    std::string front;
    std::string back;
    std::string top;
    std::string bottom;
    std::string left;
    std::string right;

    std::vector<std::string> get_paths()
    {
        std::vector<std::string> paths = {right, left, top, bottom, front, back};
        return paths;
    }
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SkyBoxComponent, identifier, front, back, top, bottom, left, right);
