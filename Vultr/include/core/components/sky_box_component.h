#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <helpers/file.h>
#include <json/json.hpp>
#include <json/glm_serializer.hpp>

struct SkyBoxComponent
{

    static SkyBoxComponent Create(const char *identifier = "default", const char *front = "textures/skybox/front.jpg", const char *back = "textures/skybox/back.jpg", const char *top = "textures/skybox/top.jpg",
                                  const char *bottom = "textures/skybox/bottom.jpg", const char *left = "textures/skybox/left.jpg", const char *right = "textures/skybox/right.jpg")
    {
        SkyBoxComponent component = SkyBoxComponent();
        component.identifier = Vultr::File(identifier, TEXTURE_FILE_EXTENSIONS);
        component.front = Vultr::File(front, TEXTURE_FILE_EXTENSIONS);
        component.back = Vultr::File(back, TEXTURE_FILE_EXTENSIONS);
        component.top = Vultr::File(top, TEXTURE_FILE_EXTENSIONS);
        component.bottom = Vultr::File(bottom, TEXTURE_FILE_EXTENSIONS);
        component.left = Vultr::File(left, TEXTURE_FILE_EXTENSIONS);
        component.right = Vultr::File(right, TEXTURE_FILE_EXTENSIONS);
        return component;
    }

    Vultr::File identifier;
    Vultr::File front;
    Vultr::File back;
    Vultr::File top;
    Vultr::File bottom;
    Vultr::File left;
    Vultr::File right;

    std::vector<std::string> get_paths()
    {
        std::vector<std::string> paths = {right.GetPath(), left.GetPath(), top.GetPath(), bottom.GetPath(), front.GetPath(), back.GetPath()};
        return paths;
    }
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SkyBoxComponent, identifier, front, back, top, bottom, left, right);
