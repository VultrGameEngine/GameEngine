#pragma once
#include "material.h"
#include <gui/core/ui_id.h>
#include <gui/core/transform.h>
#include <rendering/types/mesh.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct StencilRequest
        {
            enum Type
            {
                STENCIL,     // Stencil is the most flexible, it will do an actual stencil test and clip based on that. However it is extremely expensive and not recommended if it can be avoided.
                DEFAULT_SDF, // Takes an array of SDF functions which will be used in fragment shaders to determine if pixels are inside of the SDF. This is not very flexible,
                             // but is much more performant and also supports the default SDF functions like rounding rectangles and whatnot. If you can use this, do so over stencils.
                             // NOT IMPLEMENTED
                SCISSOR,     // Scissor is just a glScissor, so it can only take a single rectangular area and will restrict drawing to that area.
                             // This is the least flexible but also probably the most performant. It is best used in things like scrollviews and whatnot.
            };

            Type type = STENCIL;
            union StencilRequestData {
                struct
                {
                    Material *material;
                    Mesh mesh;
                };

                StencilRequestData()
                {
                    memset(this, 0, sizeof(StencilRequestData));
                }
                ~StencilRequestData(){};
            } data;

            Transform local_transform;
            UI_ID id;

            // Linked list essentially
            StencilRequest *parent = nullptr;
        };

        void destroy_stencil_request(StencilRequest *request);
    } // namespace IMGUI
} // namespace Vultr
