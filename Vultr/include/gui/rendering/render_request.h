#pragma once
#include "material.h"
#include "quad_batch.h"
#include "stencil_request.h"
#include <gui/core/ui_id.h>
#include <gui/core/transform.h>
#include <rendering/types/shader.h>
#include <rendering/types/mesh.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct RenderRequestContext
        {
            StencilRequest *stencil = nullptr;
        };

        struct RenderRequest
        {
            enum Type
            {
                MESH_DRAW,
                ABSOLUTE_MESH_DRAW,
                BATCH_DRAW,
            };
            Type type = MESH_DRAW;
            bool clip = false;
            Material *material;
            Transform local_transform;
            UI_ID id;
            s32 z_index = 0;
            StencilRequest *stencil = nullptr;

            union RenderRequestData {
                struct
                {
                    Mesh mesh;
                };
                struct
                {
                    QuadBatch *batch;
                    u32 num_quads;
                };
                RenderRequestData()
                {
                    memset(this, 0, sizeof(RenderRequestData));
                }
                ~RenderRequestData(){};
            } data;

            bool operator<(const RenderRequest &other) const;
        };

        void destroy_render_request(RenderRequest &r);
    } // namespace IMGUI
} // namespace Vultr
