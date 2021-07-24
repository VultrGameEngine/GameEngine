#pragma once
#include <types/types.hpp>
#include "shader.h"

namespace Vultr
{
#define MAX_UNIFORMS 16
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
            Mat4 u_mat4;
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
            MAT4 = 7,
            EMPTY = 8,
        };

        Type type = EMPTY;
    };

    struct Material
    {
        MaterialUniform uniforms[MAX_UNIFORMS];
    };

    MaterialUniform bool_uniform(s32 location, bool value);
    MaterialUniform u32_uniform(s32 location, u32 value);
    MaterialUniform s32_uniform(s32 location, s32 value);
    MaterialUniform f64_uniform(s32 location, f32 value);
    MaterialUniform vec2_uniform(s32 location, Vec2 value);
    MaterialUniform vec3_uniform(s32 location, Vec3 value);
    MaterialUniform vec4_uniform(s32 location, Vec4 value);
    MaterialUniform mat4_uniform(s32 location, Mat4 value);
    void material_bind_uniforms(const Material &material);
} // namespace Vultr
