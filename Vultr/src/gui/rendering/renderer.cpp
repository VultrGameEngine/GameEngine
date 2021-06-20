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

    const ShaderImporter::ShaderProgramSource TEXTURE_GUI = {
        .VertexSource = "#version 330 core\n"
                        "#extension GL_ARB_separate_shader_objects: enable\n"
                        "layout (location = 0) in vec3 position;\n"
                        "layout (location = 1) in vec3 normal; \n"
                        "layout (location = 2) in vec2 uv; \n"
                        ""
                        "out vec2 FragUV; \n"
                        ""
                        "uniform mat4 transform; \n"
                        ""
                        "void main() \n"
                        "{\n"
                        "   gl_Position = transform * vec4(position, 1.0f); \n"
                        "   FragUV = (position.xy + vec2(1, 1)) / 2.0; \n"
                        ""
                        "}\n",

        .FragmentSource = "#version 330 core\n"
                          "#extension GL_ARB_separate_shader_objects: enable\n"
                          "in vec2 FragUV; \n"
                          "out vec4 FragColor; \n"
                          ""
                          "uniform vec4 color; \n"
                          "uniform sampler2D tex; \n"
                          ""
                          "void main() \n"
                          "{\n"
                          "   FragColor = vec4(texture(tex, FragUV).rgb, 1.0); \n"
                          ""
                          "}\n",
    };

    const ShaderImporter::ShaderProgramSource BATCH_GUI = {
        .VertexSource = "#version 410 core\n"
                        "#extension GL_ARB_separate_shader_objects: enable\n"
                        "layout (location = 0) in vec3 vposition;\n"
                        "layout (location = 1) in vec4 color; \n"
                        "layout (location = 2) in vec2 uv; \n"
                        "layout (location = 3) in float texture; \n"
                        ""
                        "out vec4 fColor; \n"
                        "out vec2 fTextCoord; \n"
                        "out float fTexture; \n"
                        ""
                        "void main() \n"
                        "{\n"
                        "   gl_Position = vec4(vposition, 1.0f); \n"
                        "   fTextCoord = uv; \n"
                        "   fTexture = texture; \n"
                        "   fColor = color; \n"
                        ""
                        "}\n",

        .FragmentSource = "#version 410 core\n"
                          "#extension GL_ARB_separate_shader_objects: enable\n"
                          "out vec4 FragColor; \n"
                          ""
                          "in vec4 fColor; \n"
                          "in vec2 fTextCoord; \n"
                          "in float fTexture; \n"
                          ""
                          "uniform sampler2D u_Textures[16]; \n"
                          ""
                          "void main() \n"
                          "{\n"
                          "   if(fTexture > -1) { \n"
                          "       int index = int(fTexture); \n"
                          "       FragColor = vec4(1, 1, 1, texture(u_Textures[index], fTextCoord).r) * fColor; \n"
                          "   } else { \n"
                          "       FragColor = fColor; \n"
                          "   }"
                          ""
                          "}\n",
    };
    renderer.default_gui_shader = ShaderImporter::import_engine_shader(DEFAULT_GUI);
    renderer.texture_gui_shader = ShaderImporter::import_engine_shader(TEXTURE_GUI);
    renderer.batch_gui_shader = ShaderImporter::import_engine_shader(BATCH_GUI);
    return renderer;
}
void IMGUI::destroy_imgui_renderer(const Renderer &r)
{
    delete r.quad;
}
