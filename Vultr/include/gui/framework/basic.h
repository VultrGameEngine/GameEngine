#pragma once
#include <glm/glm.hpp>
#include "single_child_render_object_widget.h"
#include "multi_child_render_object_widget.h"
#include "stateless_widget.h"
#include "stateful_widget.h"
#include "text.h"
#include <math/lerp.h>
#include "edge_insets.h"

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
            quad_id = context->SubmitQuad();
        }

        ColoredBox *GetConfig() override
        {
            return (ColoredBox *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            Quad quad = context->GetQuad(quad_id);
            properties.color = GetConfig()->GetColor();
            properties.size = GetSize();
            properties.border_widths = GetConfig()->GetBorders();
            properties.border_color = GetConfig()->GetBorderColor();
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
        glm::vec4 color = glm::vec4(0);
        glm::vec4 border_color = glm::vec4(0);
        EdgeInsets insets;
    };

    glm::vec4 color;
    glm::vec4 border_color;
    EdgeInsets insets;

  public:
    ColoredBox(Params params)
    {
        this->key = params.key;
        this->child = params.child;
        this->color = params.color;
        this->border_color = params.border_color;
        this->insets = params.insets;
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

    glm::vec4 GetBorders() const
    {
        return insets.GetBorders();
    }

    const glm::vec4 &GetBorderColor() const
    {
        return border_color;
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

    SizedBox(Params params) : width(params.width), height(params.height)
    {
        key = params.key;
        child = params.child;
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

class AnimatedSizedBox : public SingleChildRenderObjectWidget
{
  private:
    struct Params
    {
        Widget *child = nullptr;
        Key key;
        double width = 0;
        double height = 0;
        double speed = 1;
    };

  public:
    class RenderAnimatedSizedBox : public SingleChildRenderObject
    {
      public:
        RenderAnimatedSizedBox(BuildContext *context, AnimatedSizedBox *widget)
            : SingleChildRenderObject(widget)
        {
        }

        AnimatedSizedBox *GetConfig() override
        {
            return (AnimatedSizedBox *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            context->AccumulatePosition(position);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            Size size = constraints.GetSize(GetConfig()->GetSize());
            if (layed_out)
            {
                size.width =
                    Math::Lerp(GetSize().width, size.width, GetConfig()->GetSpeed(),
                               context->GetTickInfo().m_delta_time);
                size.height = Math::Lerp(GetSize().height, size.height,
                                         GetConfig()->GetSpeed(),
                                         context->GetTickInfo().m_delta_time);
            }
            if (child != nullptr)
            {
                BoxConstraints child_constraints = BoxConstraints::Tight(size);
                child->Layout(context, child_constraints);
            }
            layed_out = true;
            return UpdateSize(size);
        }

      private:
        bool layed_out = false;
    };

    AnimatedSizedBox(Params params)
        : width(params.width), height(params.height), speed(params.speed)
    {
        key = params.key;
        child = params.child;
    }

    virtual ~AnimatedSizedBox()
    {
    }

    RenderAnimatedSizedBox *CreateRenderObject(BuildContext *context) override
    {
        return new RenderAnimatedSizedBox(context, this);
    }

    Size GetSize()
    {
        return Size(width, height);
    }
    double GetSpeed()
    {
        return speed;
    }

  protected:
    double width;
    double height;
    double speed;
};

// class BorderBox : public SingleChildRenderObjectWidget
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
    Align(Params params) : alignment(params.alignment)
    {
        this->key = params.key;
        this->child = params.child;
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

class ConstraintedBox : public SingleChildRenderObjectWidget
{
  private:
    struct Params
    {
        Widget *child;
        Key key;
        BoxConstraints constraints;
    };

    BoxConstraints constraints;

  public:
    class RenderConstrainedBox : public SingleChildRenderObject
    {
      public:
        RenderConstrainedBox(BuildContext *context, ConstraintedBox *widget)
            : SingleChildRenderObject(widget)
        {
        }

        ConstraintedBox *GetConfig() override
        {
            return (ConstraintedBox *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
            context->AccumulatePosition(position);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            if (child == nullptr)
            {
                return constraints.Max();
            }
            BoxConstraints additional_constraints =
                constraints.GenerateConstraints(GetConfig()->GetConstraints());
            return child->Layout(context, additional_constraints);
        }
    };

    ConstraintedBox(Params params) : constraints(params.constraints)
    {
        this->child = params.child;
        this->key = params.key;
    }
    BoxConstraints GetConstraints()
    {
        return constraints;
    }
    virtual ~ConstraintedBox()
    {
    }

    RenderConstrainedBox *CreateRenderObject(BuildContext *context) override
    {
        return new RenderConstrainedBox(context, this);
    }
};

class Flex : public SingleChildRenderObjectWidget
{
  private:
    struct Params
    {
        Key key;
        Widget *child;
        double flex = 1.0;
    };
    double flex;

  public:
    class RenderFlex : public SingleChildRenderObject
    {
      public:
        RenderFlex(BuildContext *context, Flex *widget)
            : SingleChildRenderObject(widget)
        {
        }

        Flex *GetConfig() override
        {
            return (Flex *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
        }

        // Special for the flex widget
        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child, bool layout) override
        {
            return Size(GetConfig()->flex);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            return child->Layout(context, constraints);
        }
    };

    Flex(Params params) : flex(params.flex)
    {
        this->key = params.key;
        this->child = params.child;
    }

    virtual ~Flex()
    {
    }

    RenderFlex *CreateRenderObject(BuildContext *context) override
    {
        return new RenderFlex(context, this);
    }
};

class Row : public MultiChildRenderObjectWidget
{
  private:
    struct Params
    {
        Key key;
        std::vector<Widget *> children;
        Alignment alignment = Alignment::Center();
    };

    Alignment alignment;

  public:
    class RenderRow : public GUI::MultiChildRenderObject
    {
      public:
        RenderRow(BuildContext *context, Row *widget)
            : MultiChildRenderObject(widget)
        {
        }

        Row *GetConfig() override
        {
            return (Row *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
        }

        void ApplyPosition(BuildContext *context, int index) override
        {
            context->AccumulatePosition(positions.at(index));
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    std::vector<Element *> children) override
        {
            double width = 0;
            double height = 0;
            std::vector<Size> child_sizes;
            int flex_total = 0;
            for (Element *child : children)
            {
                // If it is not a flex, then it will just call layout. But if it is a
                // flex then it will actually just return the flex size with the
                // value. It will then get laid out later
                Size child_size =
                    child->GetSize(context, constraints.GenerateLoose());
                if (std::isinf(child_size.width))
                {
                    flex_total += child_size.value;
                }
                else
                {
                    if (child_size.height > height)
                        height = child_size.height;
                    width += child_size.width;
                }
                child_sizes.push_back(child_size);
            }

            if (flex_total > 0)
            {
                double empty_width = constraints.max_width - width;
                width = constraints.max_width;
                for (int i = 0; i < child_sizes.size(); i++)
                {
                    Size &size = child_sizes.at(i);
                    if (std::isinf(size.width))
                    {
                        BoxConstraints flex_constraints =
                            constraints.GenerateLoose();
                        double flex_width = empty_width * (size.value / flex_total);
                        // These are now the tight constraints which the flex will
                        // properly layout its children with
                        flex_constraints.max_width = flex_width;
                        flex_constraints.min_width = flex_width;

                        // Actually layout this time
                        // This will also put the proper height in
                        size = children.at(i)->Layout(context, flex_constraints);
                        if (size.height > height)
                            height = size.height;
                    }
                }
            }

            glm::vec2 position = glm::vec2(0, 0);

            // Essentially do the same thing as align, but this time using the entire
            // dimensions of the children combined
            if (GetConfig()->alignment.x != 0 && constraints.max_width != width)
            {
                double difference = constraints.max_width - width;
                double padding = 0.5 * difference * GetConfig()->alignment.x;
                position.x = padding;
            }
            if (GetConfig()->alignment.y != 0 && constraints.max_height != height)
            {
                double difference = constraints.max_height - height;
                double padding = 0.5 * difference * GetConfig()->alignment.y;
                position.y = padding;
            }

            double current_offset = position.x - (width / 2);

            for (int i = 0; i < child_sizes.size(); i++)
            {
                assert(i <= positions.size() &&
                       "Something went wrong while laying out");
                Size child_size = child_sizes.at(i);
                glm::vec2 child_pos;
                child_pos.x = current_offset + (child_size.width / 2);
                current_offset += child_size.width;
                child_pos.y = position.y;
                if (i == positions.size())
                {
                    positions.push_back(child_pos);
                }
                else
                {
                    positions.at(i) = child_pos;
                }
            }

            return constraints.Max();
        }
    };

    Row(Params params) : alignment(params.alignment)
    {
        this->key = params.key;
        this->children = params.children;
    }

    const std::vector<Widget *> &GetChildren()
    {
        return children;
    }

    virtual ~Row()
    {
    }

    RenderRow *CreateRenderObject(BuildContext *context) override
    {
        return new RenderRow(context, this);
    }
};
class Column : public MultiChildRenderObjectWidget
{
  private:
    struct Params
    {
        Key key;
        std::vector<Widget *> children;
        Alignment alignment = Alignment::Center();
    };

    Alignment alignment;

  public:
    class RenderColumn : public GUI::MultiChildRenderObject
    {
      public:
        RenderColumn(BuildContext *context, Column *widget)
            : MultiChildRenderObject(widget)
        {
        }

        Column *GetConfig() override
        {
            return (Column *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
        }

        void ApplyPosition(BuildContext *context, int index) override
        {
            context->AccumulatePosition(positions.at(index));
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    std::vector<Element *> children) override
        {
            double height = 0;
            double width = 0;
            std::vector<Size> child_sizes;
            int flex_total = 0;
            for (Element *child : children)
            {
                // If it is not a flex, then it will just call layout. But if it is a
                // flex then it will actually just return the flex size with the
                // value. It will then get laid out later
                Size child_size =
                    child->GetSize(context, constraints.GenerateLoose());
                if (std::isinf(child_size.height))
                {
                    flex_total += child_size.value;
                }
                else
                {
                    if (child_size.width > width)
                        width = child_size.width;
                    height += child_size.height;
                }
                child_sizes.push_back(child_size);
            }

            if (flex_total > 0)
            {
                double empty_height = constraints.max_height - height;
                height = constraints.max_height;
                for (int i = 0; i < child_sizes.size(); i++)
                {
                    Size &size = child_sizes.at(i);
                    if (std::isinf(size.height))
                    {
                        BoxConstraints flex_constraints =
                            constraints.GenerateLoose();
                        double flex_height =
                            empty_height * (size.value / flex_total);
                        // These are now the tight constraints which the flex will
                        // properly layout its children with
                        flex_constraints.max_height = flex_height;
                        flex_constraints.min_height = flex_height;

                        // Actually layout this time
                        // This will also put the proper height in
                        size = children.at(i)->Layout(context, flex_constraints);
                        if (size.width > width)
                            width = size.width;
                    }
                }
            }

            glm::vec2 position = glm::vec2(0, 0);

            // Essentially do the same thing as align, but this time using the entire
            // dimensions of the children combined
            if (GetConfig()->alignment.x != 0 && constraints.max_width != width)
            {
                double difference = constraints.max_width - width;
                double padding = 0.5 * difference * GetConfig()->alignment.x;
                position.x = padding;
            }

            if (GetConfig()->alignment.y != 0 && constraints.max_height != height)
            {
                double difference = constraints.max_height - height;
                double padding = 0.5 * difference * GetConfig()->alignment.y;
                position.y = padding;
            }

            double current_offset = position.y - (height / 2);

            for (int i = 0; i < child_sizes.size(); i++)
            {
                assert(i <= positions.size() &&
                       "Something went wrong while laying out");
                Size child_size = child_sizes.at(i);
                glm::vec2 child_pos;
                child_pos.y = current_offset + (child_size.height / 2);
                current_offset += child_size.height;
                child_pos.x = position.x;
                if (i == positions.size())
                {
                    positions.push_back(child_pos);
                }
                else
                {
                    positions.at(i) = child_pos;
                }
            }

            return constraints.Max();
        }
    };

    Column(Params params) : alignment(params.alignment)
    {
        this->key = params.key;
        this->children = params.children;
    }

    const std::vector<Widget *> &GetChildren()
    {
        return children;
    }

    virtual ~Column()
    {
    }

    RenderColumn *CreateRenderObject(BuildContext *context) override
    {
        return new RenderColumn(context, this);
    }
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
        EdgeInsets borders;
        glm::vec4 border_color = glm::vec4(0);
    };

    glm::vec4 color;
    Widget *child;
    double width;
    double height;
    EdgeInsets borders;
    glm::vec4 border_color;

  public:
    Container(Params params)
    {
        this->key = params.key;
        this->child = params.child;
        this->color = params.color;
        this->width = params.width;
        this->height = params.height;

        this->borders = params.borders;
        this->border_color = params.border_color;
    }

    ~Container()
    {
    }

    Widget *Build(BuildContext *context) override
    {
        Widget *current = child;
        if (color != glm::vec4(0, 0, 0, 0) || borders.GetBorders() != glm::vec4(0))
            current = new ColoredBox({
                .child = child,
                .color = color,
                .border_color = border_color,
                .insets = borders,
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
