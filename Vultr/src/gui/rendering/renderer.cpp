#include <gui/rendering/renderer.h>
#include <helpers/mesh_importer.h>
#include <helpers/shader_importer.h>

using namespace Vultr;

IMGUI::Renderer IMGUI::new_imgui_renderer()
{
    Renderer renderer;
    renderer.quad = MeshImporter::init_quad();
    const ShaderImporter::ShaderProgramSource DEFAULT_GUI = {
        .VertexSource = "#version 330 core\n"
                        "#extension GL_ARB_separate_shader_objects: enable\n"
                        "layout (location = 0) in vec3 position;\n"
                        "layout (location = 1) in vec3 normal; \n"
                        "layout (location = 2) in vec2 uv; \n"
                        ""
                        "uniform mat4 transform; \n"
                        ""
                        "void main() \n"
                        "{\n"
                        "   gl_Position = transform * vec4(position, 1.0f); \n"
                        ""
                        "}\n",

        .FragmentSource = "#version 330 core\n"
                          "#extension GL_ARB_separate_shader_objects: enable\n"
                          "out vec4 FragColor; \n"
                          ""
                          "uniform vec4 color; \n"
                          ""
                          "void main() \n"
                          "{\n"
                          "   FragColor = color; \n"
                          ""
                          "}\n",
    };
    renderer.default_gui_shader = ShaderImporter::import_engine_shader(DEFAULT_GUI);
    return renderer;
}
void IMGUI::destroy_imgui_renderer(const Renderer &r)
{
    delete r.quad;
}
