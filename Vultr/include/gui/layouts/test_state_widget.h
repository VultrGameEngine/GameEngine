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
        glm::vec2 size = glm::vec2(1, 1);
        double font_size = 20;
        std::string text = "";
        std::string font = "res/fonts/Roboto-Regular.ttf";
    };

  public:
    glm::vec2 size;
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

    glm::vec2 GetScale()
    {
        if (mouse_down)
        {
            return glm::vec2(0.95);
        }
        else if (hovered)
        {
            return glm::vec2(1.1);
        }
        else
        {
            return glm::vec2(1);
        }
    }

    double GetBorderAlpha()
    {
        if (mouse_down || hovered)
            return 255;
        return 0;
    }
};

class TestStateWidget : public StatefulWidget<TestStateWidgetState>
{
  private:
    struct Params
    {
        Key key;
    };

  protected:
    TestStateWidget *GetWidget() override
    {
        return (TestStateWidget *)element->GetWidget();
    }
};
