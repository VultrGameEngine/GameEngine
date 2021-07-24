#pragma once
#include <glm/glm.hpp>
#include <rendering/render_type.h>
#include <types/types.hpp>
#include <fundamental/types.h>

namespace Vultr
{
    typedef u32 Shader;

    Shader invalid_shader();

    void bind_shader(Shader shader);
    void unbind_all_shaders();
    bool is_valid_shader(Shader shader);
    void delete_shader(Shader shader);

    void set_uniform_matrix_4fv(u32 location, const float *value);
    void set_uniform_4f(u32 location, const Vec4 &value);
    void set_uniform_3f(u32 location, const Vec3 &value);
    void set_uniform_2f(u32 location, const Vec2 &value);
    void set_uniform_1i(u32 location, s32 value);
    void set_uniform_1iv(u32 location, size_t size, const s32 *value);
    void set_uniform_1f(u32 location, f32 value);
    void set_uniform_bool(u32 location, bool value);

    void set_uniform_matrix_4fv(Shader shader, const char *uniform, const float *value);
    void set_uniform_4f(Shader shader, const char *uniform, const Vec4 &value);
    void set_uniform_3f(Shader shader, const char *uniform, const Vec3 &value);
    void set_uniform_2f(Shader shader, const char *uniform, const Vec2 &value);
    void set_uniform_1i(Shader shader, const char *uniform, s32 value);
    void set_uniform_1iv(Shader shader, const char *uniform, size_t size, const s32 *value);
    void set_uniform_1f(Shader shader, const char *uniform, f32 value);
    void set_uniform_bool(Shader shader, const char *uniform, f32 value);

    u32 get_uniform_location(Shader shader, const char *uniform);
} // namespace Vultr
