#pragma once
#include "gui_vertex.h"
#include <array>

namespace Vultr::GUI
{
class Constraint
{
  public:
    virtual double ApplySize(double in) = 0;
    virtual double ApplyPosition(double parent_pos, double parent_dimension) = 0;
    virtual ~Constraint() = default;

  protected:
    double value;
};
class AbsoluteConstraint : public Constraint
{
  public:
    AbsoluteConstraint(double p_value)
    {
        this->value = p_value;
    }

    double ApplySize(double in) override
    {
        return value;
    }

    double ApplyPosition(double parent_pos, double parent_dimension) override
    {
        return parent_pos + value;
    }
};

class FractionalConstraint : public Constraint
{
  public:
    FractionalConstraint(double p_value)
    {
        this->value = p_value;
    }

    double ApplySize(double in) override
    {
        return in * value;
    }

    double ApplyPosition(double parent_pos, double parent_dimension) override
    {
        double left = parent_pos - parent_dimension / 2;
        double right = parent_pos + parent_dimension / 2;

        // Get the distance between the two points and then multliply that by the
        // value to then be added to the left side
        return (right - left) * value + left;
    }
};

class EmptyConstraint : public Constraint
{

  public:
    EmptyConstraint()
    {
        this->value = 0;
    }

    // Don't need to do anything to the constraint because an empty constraint just
    // carries over the existing one
    double ApplySize(double in) override
    {
        return in;
    }

    double ApplyPosition(double parent_pos, double parent_dimension) override
    {
        return parent_pos;
    }
};
struct QuadProperties
{
    QuadProperties() = default;

    glm::vec4 color = glm::vec4(1);
    Constraint *width = new EmptyConstraint();
    Constraint *height = new EmptyConstraint();
    Constraint *x = new EmptyConstraint();
    Constraint *y = new EmptyConstraint();
    double rotation = 0;
};

class BuildContext;

class Quad
{
  public:
    Quad(GUIVertex &p_vertex1, GUIVertex &p_vertex2, GUIVertex &p_vertex3,
         GUIVertex &p_vertex4)
    {
        vertices[0] = &p_vertex1;
        vertices[1] = &p_vertex2;
        vertices[2] = &p_vertex3;
        vertices[3] = &p_vertex4;
    }

    void Commit(const QuadProperties &properties, BuildContext *context);

  private:
    std::array<GUIVertex *, 4> vertices;
};
} // namespace Vultr::GUI
