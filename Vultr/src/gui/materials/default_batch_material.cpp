#include <gui/materials/default_batch_material.h>

using namespace Vultr;

IMGUI::DefaultBatchMaterial *IMGUI::new_batch_material(Context *c, Texture *texture)
{
    auto *mat = new DefaultBatchMaterial();
    mat->shader = c->renderer.batch_gui_shader;
    mat->texture = texture;
    for (u32 i = 0; i < 16; i++)
    {
        mat->samplers[i] = 0;
    }
    return mat;
}

void IMGUI::DefaultBatchMaterial::bind()
{
    bind_shader(shader);
    bind_texture(texture, GL_TEXTURE0);
    set_uniform_1iv(shader, "u_Textures", 16, samplers);
}
