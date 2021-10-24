#include <gui/materials/default_rounded_material.h>
#include <gui/utils/opengl.h>

using namespace Vultr;

IMGUI::DefaultRoundedMaterial *IMGUI::new_rounded_material(Context *c, Color color, Vec4 corner_radius)
{
    auto *mat = new DefaultRoundedMaterial();
    mat->shader = &c->renderer.rounded_gui_shader;
    mat->color = color;
    mat->corner_radius = corner_radius;
    return mat;
}

void IMGUI::DefaultRoundedMaterial::bind()
{
    bind_shader(shader);
    set_uniform_4f(shader, "u_color", color.value);
    set_uniform_4f(shader, "u_radius", corner_radius);
}
