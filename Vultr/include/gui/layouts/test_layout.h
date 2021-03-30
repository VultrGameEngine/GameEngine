#pragma once
#include <gui/framework/basic.h>
#include <gui/framework/window.h>
#include <helpers/font_importer.h>

using namespace Vultr::GUI;
Window *TestLayout();

class RootWidget : public StatelessWidget
{
  private:
    struct Params
    {
        Key key;
    };
    Vultr::Font font;

  public:
    RootWidget(Params params)
    {
        this->key = params.key;
    }

    ~RootWidget()
    {
    }

    Widget *Build(BuildContext *context) override;
};
