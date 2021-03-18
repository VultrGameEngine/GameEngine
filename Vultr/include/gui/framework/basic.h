#pragma once
#include <glm/glm.hpp>
#include "single_child_render_object_widget.h"
#include "stateless_widget.h"

namespace Vultr
{
namespace GUI
{

class ColoredBox : public SingleChildRenderObjectWidget
{
  private:
    class RenderedColoredBox : public RenderObject
    {
      public:
        RenderedColoredBox(BuildContext *context, Widget *widget)
            : RenderObject(widget)
        {
            // TODO Fix this so that the layer is dependent on that given by the
            // parent
            vertex_index = context->SubmitQuad();
        }

        ColoredBox *GetConfig() override
        {
            return (ColoredBox *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            Quad quad = context->GetQuad(vertex_index);
            properties.color = GetConfig()->GetColor();
            quad.Commit(properties, context);
            repaint_required = false;
        }
    };

    struct Params
    {
        Key key;
        Widget *child = nullptr;
        glm::vec4 color = glm::vec4(0, 0, 0, 0);
    };

    glm::vec4 color;

  public:
    ColoredBox(Params params)
    {
        this->key = params.key;
        this->child = params.child;
        this->color = params.color;
    }

    ~ColoredBox()
    {
    }

    RenderedColoredBox *CreateRenderObject(BuildContext *context) override
    {
        return new RenderedColoredBox(context, this);
    }

    const glm::vec4 &GetColor() const
    {
        return color;
    }
};

class SizedBox : public SingleChildRenderObjectWidget
{
  private:
    struct Params
    {
        Widget *child = nullptr;
        Key key;
        Constraint *width = new EmptyConstraint();
        Constraint *height = new EmptyConstraint();
    };

  public:
    class RenderSizedBox : public RenderObject
    {
      public:
        RenderSizedBox(BuildContext *context, SizedBox *widget)
            : RenderObject(widget)
        {
        }

        SizedBox *GetConfig() override
        {
            return (SizedBox *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            ApplyConstraints(context);
            repaint_required = false;
        }

        void ApplyConstraints(BuildContext *context)
        {
            BuildContext::Accumulator current = context->GetCurrentDimensions();
            double width = this->GetConfig()->GetWidth()->ApplySize(current.size.x);
            double height = this->GetConfig()->GetWidth()->ApplySize(current.size.y);
            context->AccumulateSize(glm::vec2(width, height));
        }
    };

    SizedBox(Params params)
    {
        key = params.key;
        child = params.child;
        width = params.width;
        height = params.height;
    }

    virtual ~SizedBox()
    {
        delete width;
        delete height;
    }

    Constraint *GetWidth()
    {
        return width;
    }

    Constraint *GetHeight()
    {
        return height;
    }

  protected:
    Constraint *width;
    Constraint *height;
};

class PositionedBox : public SingleChildRenderObjectWidget
{
  private:
    struct Params
    {
        Widget *child = nullptr;
        Key key;
        Constraint *x = new EmptyConstraint();
        Constraint *y = new EmptyConstraint();
    };

  public:
    class RenderPositionedBox : public RenderObject
    {
      public:
        RenderPositionedBox(BuildContext *context, PositionedBox *widget)
            : RenderObject(widget)
        {
        }

        PositionedBox *GetConfig() override
        {
            return (PositionedBox *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            ApplyConstraints(context);
            repaint_required = false;
        }

        void ApplyConstraints(BuildContext *context)
        {
            BuildContext::Accumulator current = context->GetCurrentDimensions();
            double x = this->GetConfig()->GetX()->ApplyPosition(current.position.x,
                                                                current.size.x);
            double y = this->GetConfig()->GetY()->ApplyPosition(current.position.y,
                                                                current.size.y);
            context->AccumulateSize(glm::vec2(x, y));
        }
    };

    PositionedBox(Params params)
    {
        this->key = params.key;
        this->child = params.child;
        this->x = params.x;
        this->y = params.y;
    }

    virtual ~PositionedBox()
    {
        delete x;
        delete y;
    }

    Constraint *GetX()
    {
        return x;
    }

    Constraint *GetY()
    {
        return y;
    }

  protected:
    Constraint *x;
    Constraint *y;
};

class Container : public StatelessWidget
{
  private:
    struct Params
    {
        Key key;
        Widget *child = nullptr;
        glm::vec4 color = glm::vec4(0, 0, 0, 0);
    };

    glm::vec4 color;
    Widget *child;

  public:
    Container(Params params)
    {
        this->key = params.key;
        this->child = params.child;
        this->color = params.color;
    }

    ~Container()
    {
    }

    Widget *Build() override
    {
        if (color != glm::vec4(0, 0, 0, 0))
            return new ColoredBox({
                .child = child,
                .color = color,
            });
        return child;
    }
};
} // namespace GUI
} // namespace Vultr
