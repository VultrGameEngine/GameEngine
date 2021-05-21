#include <core/system_providers/gui_system_provider.h>
#include <core/systems/gui_system.h>
#include <core/system_providers/render_system_provider.h>
#include <engine.hpp>

namespace Vultr::GUISystem
{
    void register_system()
    {
        Signature signature;
        register_global_system<Component>(signature);
    }

    void init(GUI::Window *window)
    {
        auto &provider = get_provider();
        if (provider.context != nullptr)
        {
            delete provider.context;
        }

        provider.context = new GUI::BuildContext();
        if (provider.window != nullptr)
        {
            delete provider.window;
        }

        provider.window = window;
        provider.root_element = window->CreateElement(provider.context);
        provider.root_element->Rebuild(provider.context);
        provider.gui_shader = ShaderImporter::ImportShader("shaders/gui.glsl");
    }

    bool receive_mouse_event(Input::MouseInputEvent event)
    {
        auto &provider = get_provider();
        return provider.context->SubmitMouseInputEvent(event);
    }
    bool receive_mouse_button_event(Input::MouseButtonInputEvent event)
    {
        auto &provider = get_provider();
        return provider.context->SubmitMouseButtonInputEvent(event);
    }
    bool receive_scroll_event(Input::ScrollInputEvent event)
    {
        auto &provider = get_provider();
        return provider.context->SubmitScrollInputEvent(event);
    }

    void update(UpdateTick tick)
    {
        auto &provider = get_provider();
        provider.context->PassUpdate(tick);
        if (provider.root_element == nullptr)
            return;
        provider.root_element->Layout(provider.context, GUI::Size(RenderSystem::get_dimensions(GAME).x, RenderSystem::get_dimensions(GAME).y));
        provider.root_element->Update(provider.context);
        provider.root_element->Rebuild(provider.context);
        provider.gui_shader->Bind();
        glDisable(GL_DEPTH_TEST);
        provider.context->Draw(provider.gui_shader);
    }
} // namespace Vultr::GUISystem
