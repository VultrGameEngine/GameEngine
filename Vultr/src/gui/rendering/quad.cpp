#include <gui/rendering/quad.h>
#include <gui/framework/build_context.h>
namespace Vultr
{
namespace GUI
{
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
    // TODO add z index
    vertices[0]->position = glm::vec3(x - width / 2, y + height / 2, zindex);
    vertices[1]->position = glm::vec3(x - width / 2, y - height / 2, zindex);
    vertices[2]->position = glm::vec3(x + width / 2, y - height / 2, zindex);
    vertices[3]->position = glm::vec3(x + width / 2, y + height / 2, zindex);

    for (GUIVertex *vertex : vertices)
    {
        vertex->color = properties.color;
    }
}
} // namespace GUI
} // namespace Vultr
