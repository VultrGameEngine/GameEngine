#include <gui/rendering/quad.h>
#include <gui/framework/build_context.h>
#include <core/system_providers/render_system_provider.h>
#include <math/map.h>

namespace Vultr
{
namespace GUI
{
double translateX(double value)
{
    double w = RenderSystemProvider::GetDimensions(GAME).x;
    return (2.0 * value + 1.0) / w;
}
double translateY(double value)
{
    double h = RenderSystemProvider::GetDimensions(GAME).y;
    return (2.0 * value + 1.0) / h;
}

void Quad::Commit(const QuadProperties &properties, BuildContext *context)
{
    double width = properties.size.width;
    double height = properties.size.height;
    glm::vec2 position = context->GetPosition();
    double x = position.x;
    double y = position.y;

    // ORDER
    // Top Left [0]
    // Bottom Left [1]
    // Bottom Right [2]
    // Top Right [3]

    int zindex = context->IncreaseZ();

    // vertices[0]->position = glm::vec3(-1, 1, zindex);
    // vertices[1]->position = glm::vec3(-1, -1, zindex);
    // vertices[2]->position = glm::vec3(1, -1, zindex);
    // vertices[3]->position = glm::vec3(1, 1, zindex);
    vertices[0]->position = glm::vec3(x - width / 2, y + height / 2, zindex);
    vertices[1]->position = glm::vec3(x - width / 2, y - height / 2, zindex);
    vertices[2]->position = glm::vec3(x + width / 2, y - height / 2, zindex);
    vertices[3]->position = glm::vec3(x + width / 2, y + height / 2, zindex);

    if (texture_slot != -1)
    {
        double left = properties.uv.x;
        double right = properties.uv.x +
                       properties.uv_dimensions.x / properties.texture_dimensions.x;

        double top = properties.uv.y;
        double bottom = properties.uv.y +
                        properties.uv_dimensions.y / properties.texture_dimensions.y;

        vertices[0]->uv = glm::vec2(left, top);
        vertices[1]->uv = glm::vec2(left, bottom);
        vertices[2]->uv = glm::vec2(right, bottom);
        vertices[3]->uv = glm::vec2(right, top);
    }
    else
    {
        vertices[0]->uv = glm::vec2(0, 1);
        vertices[1]->uv = glm::vec2(0, 0);
        vertices[2]->uv = glm::vec2(1, 0);
        vertices[3]->uv = glm::vec2(1, 1);
    }

    for (int i = 0; i < 4; i++)
    {
        GUIVertex *vertex = vertices[i];
        vertex->color = properties.color / glm::vec4(255);
        vertex->texture = texture_slot;
        vertex->border_color = properties.border_color;
        vertex->position.x = translateX(vertex->position.x);
        vertex->position.y = translateY(vertex->position.y);
        if (properties.border_widths != glm::vec4(0))
        {
            vertex->borders.x = properties.border_widths.x / width;
            vertex->borders.z = 1 - properties.border_widths.y / width;
            vertex->borders.y = properties.border_widths.w / height;
            vertex->borders.w = 1 - properties.border_widths.z / height;
        }
    }
}
} // namespace GUI
} // namespace Vultr
