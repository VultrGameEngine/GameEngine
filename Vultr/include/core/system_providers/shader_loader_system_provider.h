#pragma once
#include <engine.hpp>
#include <ecs/system/system_provider.hpp>
#include <core/system_providers/light_system_provider.h>
#include <unordered_map>
#include <type_info/type_info.h>
#include <rendering/types/shader.h>
#include <rendering/types/uniform_buffer.h>
#include <filesystem/file.h>

namespace Vultr
{
    namespace ShaderLoaderSystem
    {
        struct CameraUniform
        {
            Vec4 position;
            Mat4 view_matrix;
            Mat4 projection_matrix;
            f32 bloom_threshold;
        };

        struct DirectionalLightUniform
        {
            Vec4 direction;

            Vec4 ambient;
            Vec4 diffuse;
            f32 specular;
            f32 intensity;

            u16 exists;
        };

        struct PointLightsUniform
        {
            Vec4 positions[MAX_POINT_LIGHTS];

            Vec4 constants[MAX_POINT_LIGHTS];
            Vec4 linears[MAX_POINT_LIGHTS];
            Vec4 quadratics[MAX_POINT_LIGHTS];

            Vec4 ambients[MAX_POINT_LIGHTS];
            Vec4 diffuses[MAX_POINT_LIGHTS];

            Vec4 speculars[MAX_POINT_LIGHTS];
            Vec4 intensities[MAX_POINT_LIGHTS];

            u16 count;
        };

        struct Component : public SystemProvider
        {
            std::unordered_map<std::string, Shader> loaded_shaders;

#define UNIFORM_BUFFER_COUNT 3
            UniformBuffer ubos[UNIFORM_BUFFER_COUNT] = {
                new_uniform_buffer("Camera", 0, sizeof(Vec4) + sizeof(Mat4) + sizeof(Mat4) + sizeof(f32)),
                new_uniform_buffer("DirectionalLight", 1, sizeof(Vec4) * 3 + sizeof(f32) * 2 + sizeof(u16)),
                new_uniform_buffer("PointLights", 2, (sizeof(Vec4) * 8) * MAX_POINT_LIGHTS + sizeof(u16)),
            };
        };

        Component &get_provider(Engine *e);

        Shader get_shader(Engine *e, const ShaderSource &source);

        void bind_camera_uniform(Engine *e);
        void set_camera_uniform(Engine *e, const CameraUniform &uniform);

        void bind_directional_light_uniform(Engine *e);
        void set_directional_light_uniform(Engine *e, const DirectionalLightUniform &uniform);

        void bind_point_lights_uniform(Engine *e);
        void set_point_lights_uniform(Engine *e, const PointLightsUniform &uniform);
    } // namespace ShaderLoaderSystem

    template <>
    inline const char *get_struct_name<ShaderLoaderSystem::Component>()
    {
        return "ShaderLoaderSystem";
    }
} // namespace Vultr
