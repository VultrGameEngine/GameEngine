#include <glm/gtc/type_ptr.hpp>
#include <rendering/types/shader.h>

namespace Vultr
{
    Shader invalid_shader()
    {
        return 0;
    }

    void bind_shader(Shader shader)
    {
        glUseProgram(shader);
    }

    void unbind_all_shaders()
    {
        glUseProgram(0);
    }

    bool is_valid_shader(Shader shader)
    {
        return shader > 0;
    }

    void delete_shader(Shader shader)
    {
        glDeleteProgram(shader);
    }

    void set_uniform_matrix_4fv(Shader shader, const char *uniform, const float *value)
    {
        glUniformMatrix4fv(get_uniform_location(shader, uniform), 1, GL_FALSE, value);
    }
    void set_uniform_4f(Shader shader, const char *uniform, const Vec4 &value)
    {
        glUniform4f(get_uniform_location(shader, uniform), value.x, value.y, value.z, value.w);
    }
    void set_uniform_3f(Shader shader, const char *uniform, const Vec3 &value)
    {
        glUniform3f(get_uniform_location(shader, uniform), value.x, value.y, value.z);
    }
    void set_uniform_2f(Shader shader, const char *uniform, const Vec2 &value)
    {
        glUniform2f(get_uniform_location(shader, uniform), value.x, value.y);
    }
    void set_uniform_1i(Shader shader, const char *uniform, s32 value)
    {
        glUniform1i(get_uniform_location(shader, uniform), value);
    }
    void set_uniform_1iv(Shader shader, const char *uniform, size_t size, const s32 *value)
    {
        glUniform1iv(get_uniform_location(shader, uniform), size, value);
    }
    void set_uniform_1f(Shader shader, const char *uniform, f32 value)
    {
        glUniform1f(get_uniform_location(shader, uniform), value);
    }
    u32 get_uniform_location(Shader shader, const char *uniform)
    {
        return glGetUniformLocation(shader, uniform);
    }
} // namespace Vultr
