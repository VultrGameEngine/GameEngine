#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <glm/glm.hpp>
#include <hashtable.h>
#include <rendering/types/shader.h>
#include <rendering/types/texture.h>
#include <rendering/types/mesh.h>
#include <filesystem/resource_hash.h>

namespace Vultr
{
#define MAX_SHADERS 1024
#define MAX_TEXTURES 65536
    struct RenderContext
    {
        vtl::HashTable<ResourceHash, Shader> shaders;
        vtl::HashTable<ResourceHash, Texture> textures;
        vtl::HashTable<ResourceHash, Mesh> meshes;

        RenderContext();
        ~RenderContext();
        // Vec2 dimensions;
        // TransformComponent camera_transform;
        // CameraComponent camera_component;
        // Mat4 view_matrix;
        // Mat4 projection_matrix;

        Shader *get_shader(ResourceHash res);
        Texture *get_texture(ResourceHash res);
        Mesh *get_mesh(ResourceHash res);
    };
}; // namespace Vultr
