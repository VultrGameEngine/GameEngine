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

    enum TextAlign
    {
        left,
        center,
        right,
    };

  private:
    struct Params
    {
        Key key;
        std::string text;
        std::string font;
        unsigned int max_lines = 0;
        Overflow overflow = Overflow::clip;
        TextAlign alignment = TextAlign::left;
        glm::vec4 color = glm::vec4(255);
        double size = 12;
    };
    std::string text;
    unsigned int max_lines;
    Overflow overflow;
    TextAlign alignment;
    std::string font;
    glm::vec4 color;
    double size;

  public:
    class RenderedRichText : public RenderObject
    {
      public:
        RenderedRichText(BuildContext *context, Widget *widget)
            : RenderObject(widget)
        {
            Font *font = context->GetFont(GetConfig()->font);
            font_path = GetConfig()->font;
            size = GetConfig()->size;
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
            context->IncreaseZ();
            glm::vec2 cursor = glm::vec2(0, 0);
            Font *font = context->GetFont(GetConfig()->font);

            // If the font has changed, then we need to recreate every single quad
            if (font_path != GetConfig()->font)
            {
                for (int i = 0; i < text.size(); i++)
                {
                    context->DeleteQuad(quads[i]);
                }
                delete[] quads;
                text = "";
                font_path = GetConfig()->font;
            }
            if (text != GetConfig()->text)
            {
                QuadID *new_quads = new QuadID[GetConfig()->text.size()];
                for (int i = 0; i < GetConfig()->text.size(); i++)
                {
                    char c = GetConfig()->text.at(i);
                    FontCharacter character = font->GetCharacter(c, this->size);

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
                if (GetConfig()->text.size() < text.size())
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
            int char_index = 0;
            for (int i = 0; i < num_lines; i++)
            {
                std::string line = lines[i];
                double line_width = widths[i];
                double offset = 0;
                if (GetConfig()->alignment == TextAlign::center)
                {
                    double difference = GetSize().width - line_width;
                    offset = difference / 2;
                }
                else if (GetConfig()->alignment == TextAlign::right)
                {
                    offset = GetSize().width - line_width;
                }
                glm::vec2 origin = glm::vec2(
                    context->GetPosition().x - GetSize().width / 2 + offset,
                    -font->GetHeight(size) * i - font->GetHeight(size) / 4);
                cursor = origin;
                for (char c : line)
                {
                    FontCharacter character = font->GetCharacter(c, this->size);
                    QuadID id = quads[char_index];
                    Quad quad = context->GetQuad(id);
                    QuadProperties property;
                    property.color = GetConfig()->color;
                    property.size = Size(character.size);
                    property.uv = glm::vec2(character.uv, 0);
                    property.uv_dimensions =
                        character.size *
                        glm::vec2(TEXT_SCALE_FACTOR, TEXT_SCALE_FACTOR) /
                        glm::vec2(this->size, this->size);
                    property.texture_dimensions = font->texture_dimensions;
                    context->Branch();
                    context->AccumulatePosition(glm::vec2(
                        cursor.x + character.bearing.x + character.size.x / 2,
                        cursor.y + character.bearing.y - character.size.y / 2));
                    quad.Commit(id, property, context);
                    context->ExitBranch();

                    cursor.x += character.advance.x;
                    char_index++;
                }
            }

            repaint_required = false;
        }

        Size Layout(BuildContext *context, BoxConstraints constraints) override
        {
            Size size = Size();
            Font *font = context->GetFont(GetConfig()->font);

            // Don't loop through text unless we need to
            if (text != GetConfig()->text || font_path != GetConfig()->font ||
                size != GetConfig()->size || !layed_out)
            {
                font_path = GetConfig()->font;
                size = GetConfig()->size;
                layed_out = true;

                std::vector<std::string> words;

                // Split up our string into words
                std::string current_word = "";
                for (char c : GetConfig()->text)
                {
                    current_word += c;
                    if (c == ' ')
                    {
                        words.push_back(current_word);
                        current_word = "";
                    }
                }
                if (current_word != "")
                {
                    words.push_back(current_word);
                    current_word = "";
                }

                // Get the widths of each of the words
                std::vector<double> word_widths;
                for (int i = 0; i < words.size(); i++)
                {
                    std::string word = words.at(i);
                    double width = 0;
                    for (char c : word)
                    {
                        width += font->GetCharacter(c, this->size).advance.x;
                    }
                    word_widths.push_back(width);
                }

                int current_line = 0;
                double *widths = new double[1000];
                widths[0] = 0;
                std::string *lines = new std::string[1000];
                lines[0] = "";
                // Figure out where each of the words goes
                for (int i = 0; i < words.size(); i++)
                {
                    double &width = widths[current_line];
                    if (GetConfig()->max_lines == 0 ||
                        current_line < GetConfig()->max_lines - 1)
                    {
                        if (width + word_widths[i] > constraints.max_width &&
                            lines[current_line] != "")
                        {
                            current_line++;
                            lines[current_line] = words[i];
                            widths[current_line] = word_widths[i];
                            continue;
                        }
                    }
                    width += word_widths[i];
                    lines[current_line] += words.at(i);
                }
                if (this->widths != nullptr)
                    delete[] this->widths;
                if (this->lines != nullptr)
                    delete[] this->lines;
                this->widths = new double[current_line + 1];
                this->lines = new std::string[current_line + 1];
                this->num_lines = current_line + 1;
                for (int i = 0; i < current_line + 1; i++)
                {
                    this->widths[i] = widths[i];
                    this->lines[i] = lines[i];
                }
                delete[] widths;
                delete[] lines;
            }
            if (num_lines > 1)
            {
                size.width = constraints.max_width;
            }
            else
            {
                size.width = widths[0];
            }
            size.height = font->GetHeight(this->size) * num_lines;
            return UpdateSize(constraints.GetSize(size));
        }

      private:
        QuadID *quads = nullptr;
        QuadProperties *properties = nullptr;
        std::string text = "";
        double size;
        std::string font_path;
        bool layed_out = false;
        int num_lines = 1;
        double *widths = nullptr;
        std::string *lines = nullptr;
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
            if (render_object->NeedsRepaint(context))
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
        this->alignment = params.alignment;
        this->overflow = params.overflow;
        this->size = params.size;
        this->max_lines = params.max_lines;
        this->color = params.color;
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
