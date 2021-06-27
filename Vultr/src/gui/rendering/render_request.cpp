#include <gui/rendering/render_request.h>
#include <glm/gtc/type_ptr.hpp>
#include <gui/utils/opengl.h>
#include <core/system_providers/input_system_provider.h>
#include <core/system_providers/render_system_provider.h>

using namespace Vultr;

bool IMGUI::RenderRequest::operator<(const RenderRequest &other) const
{
    return this->z_index < other.z_index;
}

void IMGUI::draw_render_request(const RenderRequest &r, Transform global_transform)
{
    Mat4 full_transform;
    if (r.type == RenderRequest::ABSOLUTE_MESH_DRAW)
    {
        // full_transform = get_transform_matrix(r.transform);
    }
    else if (r.type == RenderRequest::MESH_DRAW)
    {
        Vec3 local_scale = Vec3(gl_get_size(r.local_transform.scale), 1);
        Vec3 global_position = Vec3(gl_get_position(global_transform.position, local_scale), 0);
        Vec3 local_position = Vec3(gl_get_raw_position(r.local_transform.position), 0);
        Vec2 global_scale = glm::scale(Vec3(global_transform.scale, 1.0)) * Vec4(local_scale.x, local_scale.y, 0.0, 1.0);
        full_transform = glm::translate(global_position + local_position) * glm::scale(Vec3(global_scale, 1));
    }
    else if (r.type == RenderRequest::BATCH_DRAW)
    {
        Vec3 local_scale = Vec3(r.local_transform.scale, 1);
        Vec3 global_position = Vec3(gl_get_position(global_transform.position, local_scale), 0);
        Vec3 local_position = Vec3(r.local_transform.position, 0);
        Vec2 inverted_global_scale = Vec2(1) - global_transform.scale;
        Vec3 offset = Vec3(inverted_global_scale * (Vec2(1) - Vec2(local_scale)) * Vec2(1, -1), 0);
        full_transform = glm::translate(global_position + local_position - offset) * glm::scale(Vec3(global_transform.scale, 1.0));
    }
    else
    {
        throw("???");
        return;
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
