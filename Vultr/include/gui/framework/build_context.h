#pragma once
#include <gui/rendering/gui_render_group.h>
#include <stack>
#include <fonts/font.h>
#include <helpers/font_importer.h>
#include <core/system_providers/font_system_provider.h>
#include <core/system_providers/input_system_provider.h>
#include "input_receiver.h"
#include <core/models/update_tick.h>
#include <gui/rendering/gui_renderer.h>

namespace Vultr
{
    namespace GUI
    {

        class BuildContext
        {
          public:
            BuildContext() : tick_info(UpdateTick(0, false))
            {
                renderer = new GUIRenderer();
                positions.push(Vec2(0, 0));
                zindex.push(0);
                for (QuadID quad = 0; quad < 10000; quad++)
                {
                    available_quad_ids.push(quad);
                }
            }

            UpdateTick GetTickInfo()
            {
                return tick_info;
            }

            void PassUpdate(UpdateTick tick)
            {
                tick_info = tick;
            }

            std::stack<Vec2> positions;
            std::stack<int> zindex;

            void DeleteQuad(QuadID quad)
            {
                available_quad_ids.push(quad);
                renderer->DeleteQuad(quad);
            }

            QuadID SubmitQuad(Texture *texture = nullptr)
            {
                QuadID quad = available_quad_ids.front();
                available_quad_ids.pop();
                if (texture != nullptr)
                    renderer->SubmitTexture(quad, texture);
                return quad;
            }

            Font *GetFont(const FontSource &source)
            {
                if (fonts.find(source.path.string()) != fonts.end())
                {
                    return fonts[source.path.string()];
                }
                fonts[source.path.string()] = FontImporter::import_font(source, FontSystem::get_provider().library);
                return fonts[source.path.string()];
            }

            QuadID SubmitQuad(Font *font)
            {
                return SubmitQuad(font->texture);
            }

            void CommitQuad(QuadID id, GUIVertex vertices[4])
            {
                renderer->CommitVertices(id, vertices);
            }

            Quad GetQuad(QuadID id)
            {
                return renderer->GetQuad(id);
            }

            void Draw(Shader *shader)
            {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                renderer->Draw(shader);

                for (int i = 0; i < zindex.size(); i++)
                {
                    zindex.pop();
                }
                for (int i = 0; i < positions.size(); i++)
                {
                    positions.pop();
                }
                zindex.push(0);
                positions.push(Vec2(0, 0));
            }

            Vec2 GetPosition() const
            {
                return positions.top();
            }

            void ExitBranch()
            {
                assert(positions.size() > 1 && zindex.size() > 1 && "Cannot pop!");
                positions.pop();
                zindex.pop();
            }

            void Branch()
            {
                positions.push(positions.top());
                zindex.push(zindex.top());
            }

            void AccumulatePosition(Vec2 position, bool branch = false)
            {
                positions.top() += position;
            }

            void SetPosition(Vec2 position)
            {
                positions.top() = position;
            }

            int IncreaseZ()
            {
                return zindex.top()--;
            }

            void SubmitInputReceiver(int p_zindex, InputReceiver *receiver)
            {
                input_receivers[-p_zindex] = receiver;
            }

            void DeleteInputReceiver(int p_zindex)
            {
                assert(input_receivers.find(-p_zindex) == input_receivers.end() && "Could not delete input receiver");
                input_receivers.erase(-p_zindex);
            }

            bool SubmitMouseInputEvent(MousePositionEvent event)
            {
                for (auto [z, receiver] : input_receivers)
                {
                    if (receiver->ReceiveMouseEvent(event))
                        return true;
                }
                return false;
            }
            bool SubmitMouseButtonInputEvent(MouseButtonEvent event)
            {
                for (auto [z, receiver] : input_receivers)
                {
                    if (receiver->ReceiveMouseButtonEvent(event))
                        return true;
                }
                return false;
            }
            bool SubmitScrollInputEvent(ScrollEvent event)
            {
                for (auto [z, receiver] : input_receivers)
                {
                    if (receiver->ReceiveScrollEvent(event))
                        return true;
                }
                return false;
            }

          private:
            std::unordered_map<std::string, Font *> fonts;
            std::map<unsigned int, InputReceiver *> input_receivers;
            std::queue<QuadID> available_quad_ids;
            GUIRenderer *renderer;
            UpdateTick tick_info;
        };
    } // namespace GUI
} // namespace Vultr
