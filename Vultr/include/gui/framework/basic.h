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
    class RenderedColoredBox : public SingleChildRenderObject
    {
      public:
        RenderedColoredBox(BuildContext *context, Widget *widget)
            : SingleChildRenderObject(widget)
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
            properties.size = GetSize();
            quad.Commit(properties, context);
            repaint_required = false;
            context->AccumulatePosition(position);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            if (child != nullptr)
            {
                return UpdateSize(child->Layout(context, constraints));
            }
            else
            {
                return UpdateSize(constraints.Max());
            }
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
        double width = 0;
        double height = 0;
    };

  public:
    class RenderSizedBox : public SingleChildRenderObject
    {
      public:
        RenderSizedBox(BuildContext *context, SizedBox *widget)
            : SingleChildRenderObject(widget)
        {
        }

        SizedBox *GetConfig() override
        {
            return (SizedBox *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
            context->AccumulatePosition(position);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            Size size = constraints.GetSize(GetConfig()->GetSize());
            if (child != nullptr)
            {
                BoxConstraints child_constraints = BoxConstraints::Tight(size);
                child->Layout(context, child_constraints);
            }
            return UpdateSize(size);
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
    }

    RenderSizedBox *CreateRenderObject(BuildContext *context) override
    {
        return new RenderSizedBox(context, this);
    }

    Size GetSize()
    {
        return Size(width, height);
    }

  protected:
    double width;
    double height;
};

// class PositionedBox : public SingleChildRenderObjectWidget
// {
//   private:
//     struct Params
//     {
//         Widget *child = nullptr;
//         Key key;
//         double x = 0;
//         double y = 0;
//     };

//   public:
//     class RenderPositionedBox : public RenderObject
//     {
//       public:
//         RenderPositionedBox(BuildContext *context, PositionedBox *widget)
//             : RenderObject(widget)
//         {
//         }

//         PositionedBox *GetConfig() override
//         {
//             return (PositionedBox *)configuration;
//         }

//         void Paint(BuildContext *context) override
//         {
//             repaint_required = false;
//         }
//     };

//     PositionedBox(Params params)
//     {
//         this->key = params.key;
//         this->child = params.child;
//         this->x = params.x;
//         this->y = params.y;
//     }

//     virtual ~PositionedBox()
//     {
//         delete x;
//         delete y;
//     }

//     Constraint *GetX()
//     {
//         return x;
//     }

//     Constraint *GetY()
//     {
//         return y;
//     }

//     RenderPositionedBox *CreateRenderObject(BuildContext *context) override
//     {
//         return new RenderPositionedBox(context, this);
//     }

//   protected:
//     Constraint *x;
//     Constraint *y;
// };

class Center : public SingleChildRenderObjectWidget
{
  private:
    struct Params
    {
        Widget *child = nullptr;
        Key key;
    };

  public:
    class RenderCenter : public SingleChildRenderObject
    {
      public:
        RenderCenter(BuildContext *context, Center *widget)
            : SingleChildRenderObject(widget)
        {
        }

        Center *GetConfig() override
        {
            return (Center *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
            context->AccumulatePosition(position);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            if (child != nullptr)
            {
                child->Layout(context, constraints.GenerateLoose());
            }
            return constraints.Max();
        }
    };
    Center(Params params)
    {
        this->key = params.key;
        this->child = params.child;
    }

    virtual ~Center()
    {
    }

    RenderCenter *CreateRenderObject(BuildContext *context) override
    {
        return new RenderCenter(context, this);
    }
};

struct Alignment
{
    Alignment() : x(0), y(0)
    {
    }
    Alignment(double p_x, double p_y) : x(p_x), y(p_y)
    {
    }
    double x = 0;
    double y = 0;

    static Alignment BottomCenter()
    {
        return Alignment(0, -1);
    }
    static Alignment BottomRight()
    {
        return Alignment(1, -1);
    }
    static Alignment BottomLeft()
    {
        return Alignment(-1, -1);
    }
    static Alignment Center()
    {
        return Alignment(0, 0);
    }
    static Alignment CenterRight()
    {
        return Alignment(1, 0);
    }
    static Alignment CenterLeft()
    {
        return Alignment(-1, 0);
    }
    static Alignment TopCenter()
    {
        return Alignment(0, 1);
    }
    static Alignment TopRight()
    {
        return Alignment(1, 1);
    }
    static Alignment TopLeft()
    {
        return Alignment(-1, 1);
    }
};

class Align : public SingleChildRenderObjectWidget
{
  private:
    struct Params
    {
        Widget *child = nullptr;
        Key key;
        Alignment alignment = Alignment::Center();
    };

    Alignment alignment;

  public:
    class RenderAlign : public SingleChildRenderObject
    {
      public:
        RenderAlign(BuildContext *context, Align *widget)
            : SingleChildRenderObject(widget)
        {
        }

        Align *GetConfig() override
        {
            return (Align *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
            context->AccumulatePosition(position);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            if (child != nullptr)
            {
                Size parent_size = constraints.Max();
                Size child_size =
                    child->Layout(context, constraints.GenerateLoose());
                if (GetConfig()->alignment.x != 0 &&
                    parent_size.width != child_size.width)
                {
                    double difference = parent_size.width - child_size.width;
                    double padding = 0.5 * difference * GetConfig()->alignment.x;
                    position.x = padding;
                }
                if (GetConfig()->alignment.y != 0 &&
                    parent_size.height != child_size.height)
                {
                    double difference = parent_size.height - child_size.height;
                    double padding = 0.5 * difference * GetConfig()->alignment.y;
                    position.y = padding;
                }
            }
            return constraints.Max();
        }
    };
    Align(Params params)
    {
        this->key = params.key;
        this->child = params.child;
        this->alignment = params.alignment;
    }

    virtual ~Align()
    {
    }

    RenderAlign *CreateRenderObject(BuildContext *context) override
    {
        return new RenderAlign(context, this);
    }
};

class Padding : public SingleChildRenderObjectWidget
{
  private:
    struct Params
    {
        Widget *child = nullptr;
        Key key;
        double left = 0;
        double right = 0;
        double top = 0;
        double bottom = 0;
    };

  public:
    class RenderPaddingBox : public SingleChildRenderObject
    {
      public:
        RenderPaddingBox(BuildContext *context, Padding *widget)
            : SingleChildRenderObject(widget)
        {
        }

        Padding *GetConfig() override
        {
            return (Padding *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
            context->AccumulatePosition(position);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            double left = GetConfig()->GetLeft();
            double right = GetConfig()->GetRight();
            double top = GetConfig()->GetTop();
            double bottom = GetConfig()->GetBottom();
            position = glm::vec2(left - right, bottom - top);
            if (child == nullptr)
            {
                return UpdateSize(
                    constraints.GetSize(Size(left + right, top + bottom)));
            }
            double width = constraints.max_width - left - right;
            if (width < 0)
                width = 0;
            double height = constraints.max_height - top - bottom;
            if (height < 0)
                height = 0;

            BoxConstraints child_constraints =
                BoxConstraints::Loose(Size(width, height));

            Size child_size = child->Layout(context, child_constraints);
            return UpdateSize(Size(child_size.width + left + right,
                                   child_size.height + top + bottom));
        }
    };

    Padding(Params params)
    {
        this->key = params.key;
        this->child = params.child;
        this->top = params.top;
        this->bottom = params.bottom;
        this->left = params.left;
        this->right = params.right;
    }

    virtual ~Padding()
    {
    }

    double GetLeft() const
    {
        return left;
    }
    double GetRight() const
    {
        return right;
    }
    double GetTop() const
    {
        return top;
    }
    double GetBottom() const
    {
        return bottom;
    }

    RenderPaddingBox *CreateRenderObject(BuildContext *context) override
    {
        return new RenderPaddingBox(context, this);
    }

  protected:
    double left;
    double right;
    double top;
    double bottom;
};

class Container : public StatelessWidget
{
  private:
    struct Params
    {
        Key key;
        Widget *child = nullptr;
        double width = 0;
        double height = 0;
        glm::vec4 color = glm::vec4(0, 0, 0, 0);
    };

    glm::vec4 color;
    Widget *child;
    double width;
    double height;

  public:
    Container(Params params)
    {
        this->key = params.key;
        this->child = params.child;
        this->color = params.color;
        this->width = params.width;
        this->height = params.height;
    }

    ~Container()
    {
    }

    Widget *Build() override
    {
        Widget *current;
        if (color != glm::vec4(0, 0, 0, 0))
            current = new ColoredBox({
                .child = child,
                .color = color,
            });
        if (width != 0 || height != 0)
            current = new SizedBox({
                .child = current,
                .width = width,
                .height = height,
            });
        if (current == nullptr)
        {
            return new Center({});
        }
        return current;
    }
};
} // namespace GUI
} // namespace Vultr
