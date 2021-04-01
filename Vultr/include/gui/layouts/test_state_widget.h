#pragma once
#include <gui/framework/basic.h>

using namespace Vultr::GUI;

class TestStateWidgetState : public State
{
  public:
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

  public:
    TestStateWidget(Params params) : StatefulWidget(params.key)
    {
    }
};
