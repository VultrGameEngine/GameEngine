#pragma once
#include "material.h"
#include "quad_batch.h"
#include <gui/core/ui_id.h>
#include <rendering/models/shader.h>
#include <rendering/models/mesh.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct RenderRequest
        {
            enum Type
            {
                MESH_DRAW,
                ABSOLUTE_MESH_DRAW,
                BATCH_DRAW,
            };
            Type type = MESH_DRAW;
            Material *material;
            glm::mat4 transform;
            UI_ID id;

            union RenderRequestData {
                struct
                {
                    Mesh *mesh;
                };
                struct
                {
                    QuadBatch *batch;
                    u32 num_quads;
                };
                RenderRequestData()
                {
                    memset(this, 0, sizeof(RenderRequestData));
                };
                ~RenderRequestData(){};
            } data;
        };

        void draw_render_request(const RenderRequest &r, glm::mat4 transform);
        void destroy_render_request(RenderRequest &r);
    } // namespace IMGUI
} // namespace Vultr
