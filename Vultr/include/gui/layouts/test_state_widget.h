#pragma once
#include <gui/framework/basic.h>

class TestStateWidgetState;
class TestStateWidget : public Vultr::GUI::StatefulWidget<TestStateWidgetState>
{
  private:
    struct Params
    {
        Vultr::GUI::Key key;
        Vultr::Vec2 size = Vultr::Vec2(400, 200);
        double font_size = 12;
        std::string text = "Submit";
        std::string font = "fonts/Roboto-Regular.ttf";
    };

  public:
    Vultr::Vec2 size;
    double font_size;
    std::string text;
    std::string font;
    TestStateWidget(Params params) : StatefulWidget(params.key), size(params.size), font_size(params.font_size), text(params.text), font(params.font)
    {
    }
};

class TestStateWidgetState : public Vultr::GUI::State
{
  public:
    TestStateWidgetState(Vultr::GUI::Element *element) : State(element)
    {
    }
    Vultr::GUI::Widget *Build(Vultr::GUI::BuildContext *context) override;

  private:
    bool hovered = false;
    bool mouse_down = false;

    Vultr::Vec2 GetScale()
    {
        if (mouse_down)
        {
            return Vultr::Vec2(0.95);
        }
        else if (hovered)
        {
            return Vultr::Vec2(1.1);
        }
        else
        {
            return Vultr::Vec2(1);
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
