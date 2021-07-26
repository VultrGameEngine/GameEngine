#pragma once
#include <core/models/color.h>
#include <functional>
#include <types/types.hpp>
#include "shader.h"
#include <json/json_fwd.hpp>
#include <helpers/file.h>

namespace Vultr
{
#define MAX_UNIFORMS 16
#define MAX_MATERIAL_TEXTURES 8
    struct MaterialUniform
    {

        union Data {
            bool u_bool;
            u32 u_u32;
            s32 u_s32;
            f64 u_f64;
            Vec2 u_vec2;
            Vec3 u_vec3;
            Vec4 u_vec4;
            Color u_color;
            Mat4 u_mat4;
            Data()
            {
                memset(this, 0, sizeof(Data));
            }
            ~Data(){};
        } data;

        enum Type : u8
        {
            BOOL = 0,
            U32 = 1,
            S32 = 2,
            F64 = 3,
            VEC2 = 4,
            VEC3 = 5,
            VEC4 = 6,
            COLOR = 7,
            MAT4 = 8,
            EMPTY = 9,
        };

        Type type = EMPTY;
        std::string location;
    };
    // typedef std::function<void(Shader)> MaterialBindCallback;

    struct Material
    {
        struct TextureResource
        {
            TextureSource file;
            std::string location = "";
        };

        MaterialUniform uniforms[MAX_UNIFORMS];
        size_t uniform_count = 0;

        ShaderSource shader_source;

        TextureResource textures[MAX_MATERIAL_TEXTURES];
        size_t texture_count = 0;

        // If you need more than 16 uniform locations or if you have something like a struct, then just use this callback
        // MaterialBindCallback bind_callback;
    };

    void bool_uniform(Material &material, const char *location, bool value);
    void u32_uniform(Material &material, const char *location, u32 value);
    void s32_uniform(Material &material, const char *location, s32 value);
    void f64_uniform(Material &material, const char *location, f32 value);
    void vec2_uniform(Material &material, const char *location, Vec2 value);
    void vec3_uniform(Material &material, const char *location, Vec3 value);
    void vec4_uniform(Material &material, const char *location, Vec4 value);
    void color_uniform(Material &material, const char *location, Color value);
    void mat4_uniform(Material &material, const char *location, Mat4 value);
    void texture_uniform(Material &material, const char *location, const TextureSource &source);

    void material_bind_uniforms(const Material &material, Shader shader);

    // void to_json(json &j, const Material &m);
    // void from_json(const json &j, Material &m);
} // namespace Vultr
