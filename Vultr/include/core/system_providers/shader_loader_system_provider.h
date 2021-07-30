#pragma once
#include <engine.hpp>
#include <ecs/system/system_provider.hpp>
#include <unordered_map>
#include <type_info/type_info.h>
#include <rendering/types/shader.h>
#include <rendering/types/uniform_buffer.h>
#include <helpers/file.h>

namespace Vultr
{
    namespace ShaderLoaderSystem
    {
        struct CameraUniform
        {
            Vec4 position;
            Mat4 view_matrix;
            Mat4 projection_matrix;
        };

        struct LightUniform
        {
        };

        struct Component : public SystemProvider
        {
            std::unordered_map<std::string, Shader> loaded_shaders;

#define UNIFORM_BUFFER_COUNT 1
            UniformBuffer ubos[UNIFORM_BUFFER_COUNT] = {new_uniform_buffer("Camera", 0, sizeof(Vec4) + sizeof(Mat4) + sizeof(Mat4))};
        };

        Component &get_provider(Engine *e);

        Shader get_shader(Engine *e, const ShaderSource &source);

        void bind_camera_uniform(Engine *e);
        void set_camera_uniform(Engine *e, const CameraUniform &uniform);
    } // namespace ShaderLoaderSystem

    template <>
    inline const char *get_struct_name<ShaderLoaderSystem::Component>()
    {
        return "ShaderLoaderSystem";
    }
} // namespace Vultr
