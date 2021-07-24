#include <rendering/types/material.h>
#include <glm/gtc/type_ptr.hpp>

namespace Vultr
{
    MaterialUniform bool_uniform(s32 location, bool value)
    {
        MaterialUniform uniform;
        uniform.type = MaterialUniform::BOOL;
        uniform.data.u_bool = value;
        return uniform;
    }
    MaterialUniform u32_uniform(s32 location, u32 value)
    {
        MaterialUniform uniform;
        uniform.type = MaterialUniform::U32;
        uniform.data.u_u32 = value;
        return uniform;
    }
    MaterialUniform s32_uniform(s32 location, s32 value)
    {
        MaterialUniform uniform;
        uniform.type = MaterialUniform::S32;
        uniform.data.u_s32 = value;
        return uniform;
    }
    MaterialUniform f64_uniform(s32 location, f32 value)
    {
        MaterialUniform uniform;
        uniform.type = MaterialUniform::F64;
        uniform.data.u_f64 = value;
        return uniform;
    }
    MaterialUniform vec2_uniform(s32 location, Vec2 value)
    {
        MaterialUniform uniform;
        uniform.type = MaterialUniform::VEC2;
        uniform.data.u_vec2 = value;
        return uniform;
    }
    MaterialUniform vec3_uniform(s32 location, Vec3 value)
    {
        MaterialUniform uniform;
        uniform.type = MaterialUniform::VEC3;
        uniform.data.u_vec3 = value;
        return uniform;
    }
    MaterialUniform vec4_uniform(s32 location, Vec4 value)
    {
        MaterialUniform uniform;
        uniform.type = MaterialUniform::VEC4;
        uniform.data.u_vec4 = value;
        return uniform;
    }
    MaterialUniform mat4_uniform(s32 location, Mat4 value)
    {
        MaterialUniform uniform;
        uniform.type = MaterialUniform::MAT4;
        uniform.data.u_mat4 = value;
        return uniform;
    }

    void material_bind_uniforms(const Material &material)
    {
        for (s16 i = 0; i < MAX_UNIFORMS; i++)
        {
            auto &uniform = material.uniforms[i];
            switch (uniform.type)
            {
                case MaterialUniform::BOOL:
                    set_uniform_1i(i, uniform.data.u_bool ? 1 : 0);
                    break;
                case MaterialUniform::U32:
                    set_uniform_1i(i, uniform.data.u_u32);
                    break;
                case MaterialUniform::S32:
                    set_uniform_1i(i, uniform.data.u_s32);
                    break;
                case MaterialUniform::F64:
                    set_uniform_1f(i, uniform.data.u_f64);
                    break;
                case MaterialUniform::VEC2:
                    set_uniform_2f(i, uniform.data.u_vec2);
                    break;
                case MaterialUniform::VEC3:
                    set_uniform_3f(i, uniform.data.u_vec3);
                    break;
                case MaterialUniform::VEC4:
                    set_uniform_4f(i, uniform.data.u_vec4);
                    break;
                case MaterialUniform::MAT4:
                    set_uniform_matrix_4fv(i, glm::value_ptr(uniform.data.u_mat4));
                    break;
                case MaterialUniform::EMPTY:
                    // Nothing to do here, as this is the default
                    break;
            }
        }
    }
} // namespace Vultr
