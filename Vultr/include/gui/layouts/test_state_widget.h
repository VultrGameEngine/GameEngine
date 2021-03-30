#pragma once
#include <gui/framework/basic.h>

using namespace Vultr::GUI;

class TestStateWidgetState : public State
{
  public:
    Widget *Build(BuildContext *context) override;

  private:
    std::string text = "";
};
class TestStateWidget : public StatefulWidget<TestStateWidgetState>
{
  private:
    struct Params
    {
        Key key;
    };

  public:
    TestStateWidget(Params params)
        : StatefulWidget(params.key, new TestStateWidgetState())
    {
    }
};
