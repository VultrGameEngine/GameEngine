#pragma once
#include <gui/framework/basic.h>

using namespace Vultr::GUI;

class TestStateWidgetState;
class TestStateWidget : public StatefulWidget<TestStateWidgetState>
{
  private:
    struct Params
    {
        Key key;
        Vec2 size = Vec2(400, 200);
        double font_size = 12;
        std::string text = "Submit";
        std::string font = "fonts/Roboto-Regular.ttf";
    };

  public:
    Vec2 size;
    double font_size;
    std::string text;
    std::string font;
    TestStateWidget(Params params)
        : StatefulWidget(params.key), size(params.size), font_size(params.font_size),
          text(params.text), font(params.font)
    {
    }
};

class TestStateWidgetState : public State
{
  public:
    TestStateWidgetState(Element *element) : State(element)
    {
    }
    Widget *Build(BuildContext *context) override;

  private:
    bool hovered = false;
    bool mouse_down = false;

    Vec2 GetScale()
    {
        if (mouse_down)
        {
            return Vec2(0.95);
        }
        else if (hovered)
        {
            return Vec2(1.1);
        }
        else
        {
            return Vec2(1);
        }
    }

    double GetBorderAlpha()
    {
        if (mouse_down || hovered)
            return 255;
        return 0;
    }

    TestStateWidget *GetWidget() override
    {
        return (TestStateWidget *)element->GetWidget();
    }
};
