#include <gui/rendering/quad.h>
#include <gui/framework/build_context.h>
namespace Vultr
{
namespace GUI
{
void Quad::Commit(const QuadProperties &properties, BuildContext *context)
{
    BuildContext::Accumulator current = context->GetCurrentDimensions();
    double width = properties.height->ApplySize(current.size.x);
    double height = properties.height->ApplySize(current.size.y);
    double x = properties.x->ApplyPosition(current.position.x, current.size.x);
    double y = properties.y->ApplyPosition(current.position.y, current.size.y);

    // ORDER
    // Top Left [0]
    // Bottom Left [1]
    // Bottom Right [2]
    // Top Right [3]

    // TODO add z index
    vertices[0]->position = glm::vec3(x - width / 2, y + height / 2, 0);
    vertices[1]->position = glm::vec3(x - width / 2, y - height / 2, 0);
    vertices[2]->position = glm::vec3(x + width / 2, y - height / 2, 0);
    vertices[3]->position = glm::vec3(x + width / 2, y + height / 2, 0);

    for (GUIVertex *vertex : vertices)
    {
        vertex->color = properties.color;
    }
}
} // namespace GUI
} // namespace Vultr
