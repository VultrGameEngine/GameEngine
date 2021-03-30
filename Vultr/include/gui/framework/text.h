#pragma once
#include "multi_child_render_object_widget.h"
#include <fonts/font.h>
#include <helpers/texture_importer.h>

namespace Vultr
{
namespace GUI
{
class RichText : public RenderObjectWidget
{
  public:
    enum Overflow
    {
        clip,
        ellipses,
    };

  private:
    struct Params
    {
        Key key;
        std::string text;
        Font *font;
        unsigned int max_lines = INFINITY;
        Overflow overflow = Overflow::clip;
    };
    std::string text;
    unsigned int max_lines;
    Overflow overflow;
    Font *font;

  public:
    class RenderedRichText : public RenderObject
    {
      public:
        RenderedRichText(BuildContext *context, Widget *widget)
            : RenderObject(widget)
        {
        }

        RichText *GetConfig() override
        {
            return (RichText *)configuration;
        }

        void DeleteRenderObject(BuildContext *context) override
        {
            for (int i = 0; i < text.size(); i++)
            {
                context->DeleteQuad(quads[i]);
            }
            delete[] quads;
        }

        void Paint(BuildContext *context) override
        {
            glm::vec2 cursor = glm::vec2(0, 0);
            Font *font = GetConfig()->font;
            if (text != GetConfig()->text)
            {
                QuadID *new_quads = new QuadID[GetConfig()->text.size()];
                for (int i = 0; i < GetConfig()->text.size(); i++)
                {
                    char c = GetConfig()->text.at(i);
                    FontCharacter character = GetConfig()->font->characters[c];

                    // If the new text is longer
                    if (i > text.size() - 1 || quads == nullptr)
                    {
                        new_quads[i] = context->SubmitQuad(font);
                    }
                    else
                    {
                        new_quads[i] = quads[i];
                    }
                }

                // If the new text is actually shorter
                if (GetConfig()->text.size() > text.size())
                {
                    for (int i = GetConfig()->text.size(); i < text.size(); i++)
                    {
                        context->DeleteQuad(quads[i]);
                    }
                }
                if (quads != nullptr)
                {
                    delete[] quads;
                }
                quads = new_quads;
                text = GetConfig()->text;
            }

            glm::vec2 origin =
                glm::vec2(context->GetPosition().x - GetSize().width / 2,
                          -GetSize().height / 4);
            cursor = origin;
            for (int i = 0; i < text.size(); i++)
            {
                char c = text.at(i);
                FontCharacter character = GetConfig()->font->characters[c];
                QuadID id = quads[i];
                Quad quad = context->GetQuad(id);
                QuadProperties property;
                property.color = glm::vec4(0, 0.2, 0, 1);
                property.size = Size(character.size);
                property.uv = glm::vec2(character.uv, 0);
                property.uv_dimensions = character.size * glm::vec2(2000, 2000);
                property.texture_dimensions = font->texture_dimensions;
                context->Branch();
                context->AccumulatePosition(glm::vec2(
                    cursor.x + character.bearing.x + character.size.x / 2,
                    cursor.y + character.bearing.y - character.size.y / 2));
                quad.Commit(property, context);
                context->ExitBranch();

                cursor.x += character.advance.x;
                // context->SetPosition(origin);
            }

            repaint_required = false;
        }

        Size Layout(BuildContext *context, BoxConstraints constraints) override
        {
            Size size = Size();
            glm::vec2 cursor = glm::vec2(0, 0);
            Font *font = GetConfig()->font;
            std::string current_string = "";
            for (int i = 0; i < GetConfig()->text.size(); i++)
            {
                char c = GetConfig()->text.at(i);
                FontCharacter character = font->characters[c];
                // if (GetConfig()->overflow == Overflow::wrap)
                // {
                //     if (cursor.x + character.advance.x > constraints.max_width)
                //     {
                //         // Find the last instance of a space, so that we can wrap
                //         std::size_t found = current_string.rfind(" ");
                //     }
                // }
                cursor.x += character.advance.x;
                current_string += c;

                double height = character.size.y +
                                (character.size.y / 2 - abs(character.bearing.y));

                if (cursor.x > size.width)
                    size.width = cursor.x;
            }
            size.height = font->height;
            return UpdateSize(constraints.GetSize(size));
        }

      private:
        QuadID *quads = nullptr;
        QuadProperties *properties = nullptr;
        std::string text = "";
        std::string *lines;
    };
    class TextElement : public RenderObjectElement
    {
      public:
        TextElement(BuildContext *context, RichText *p_widget)
            : RenderObjectElement(context, p_widget)
        {
        }

        void Update(BuildContext *context) override
        {
            if (render_object->NeedsRepaint())
            {
                render_object->Paint(context);
            }
        }

        Size Layout(BuildContext *context, BoxConstraints constraints) override
        {
            assert(render_object != nullptr &&
                   "No render object attached to this element");
            return ((RenderedRichText *)render_object)->Layout(context, constraints);
        }

        Size GetSize(BuildContext *context, BoxConstraints constraints) override
        {
            assert(render_object != nullptr &&
                   "No render object attached to this element");
            return ((RenderedRichText *)render_object)->Layout(context, constraints);
        }

        void Reattach(Widget *self) override
        {
            widget = self;
            render_object->Reattach(widget);
            render_object->MarkForRepaint();
        }

        void Rebuild(BuildContext *context) override
        {
        }

        RichText *GetWidget() const override
        {
            return (RichText *)widget;
        }

      private:
        ~TextElement()
        {
        }
    };

  public:
    RichText(Params params)
    {
        this->key = params.key;
        this->font = params.font;
        this->text = params.text;
        this->overflow = params.overflow;
        this->max_lines = params.max_lines;
    }

    ~RichText()
    {
    }

    TextElement *CreateElement(BuildContext *context) override
    {
        return new TextElement(context, this);
    }

    RenderedRichText *CreateRenderObject(BuildContext *context) override
    {
        return new RenderedRichText(context, this);
    }

    WidgetTypeGetter(SingleChildRenderObjectWidget);
}; // namespace GUI
} // namespace GUI
} // namespace Vultr
