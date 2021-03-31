#include <core/systems/gui_system.h>
#include <core/system_providers/render_system_provider.h>

namespace Vultr
{
void GUISystem::RegisterSystem()
{
    Signature signature;
    World::RegisterSystem<GUISystemProvider>(signature);
}

void GUISystem::Init(GUI::Window *window)
{
    GUISystemProvider &provider = GetProvider();
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
    provider.gui_shader = ShaderImporter::ImportShader("res/shaders/gui.glsl");
}

bool GUISystem::ReceiveMouseEvent(Input::MouseInputEvent event)
{
    GUISystemProvider &provider = GetProvider();
    return provider.context->SubmitMouseInputEvent(event);
}
bool GUISystem::ReceiveMouseButtonEvent(Input::MouseButtonInputEvent event)
{
    GUISystemProvider &provider = GetProvider();
    return provider.context->SubmitMouseButtonInputEvent(event);
}

void GUISystem::Update(UpdateTick tick)
{
    GUISystemProvider &provider = GetProvider();
    provider.context->PassUpdate(tick);
    if (provider.root_element == nullptr)
        return;
    provider.root_element->Layout(provider.context, GUI::Size(2, 2));
    provider.root_element->Update(provider.context);
    provider.root_element->Rebuild(provider.context);
    provider.gui_shader->Bind();
    provider.context->Draw(provider.gui_shader);
}
} // namespace Vultr
