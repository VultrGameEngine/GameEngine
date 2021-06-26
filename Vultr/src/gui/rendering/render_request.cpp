#include <gui/rendering/render_request.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Vultr;

void IMGUI::draw_render_request(const RenderRequest &r, glm::mat4 transform)
{
    glm::mat4 full_transform;
    if (r.type == RenderRequest::ABSOLUTE_MESH_DRAW)
    {
        full_transform = r.transform;
    }
    else
    {
        full_transform = transform * r.transform;
    }
    r.material->bind();
    r.material->shader->SetUniformMatrix4fv("transform", glm::value_ptr(full_transform));

    if (r.type == RenderRequest::ABSOLUTE_MESH_DRAW || r.type == RenderRequest::MESH_DRAW)
    {
        r.data.mesh->Draw();
    }
    else
    {
        quad_batch_draw(r.data.batch, r.data.num_quads);
    }
}

void IMGUI::destroy_render_request(RenderRequest &r)
{
    delete r.material;
}
