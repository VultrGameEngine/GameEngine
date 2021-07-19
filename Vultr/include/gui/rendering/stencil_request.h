#pragma once
#include "material.h"
#include <gui/core/ui_id.h>
#include <gui/core/transform.h>
#include <rendering/models/mesh.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct StencilRequest
        {
            Material *material;
            Transform local_transform;
            UI_ID id;

            // Linked list essentially
            StencilRequest *parent = nullptr;

            Mesh *mesh;
        };

        void destroy_stencil_request(StencilRequest *request);
    } // namespace IMGUI
} // namespace Vultr
