#include <core/systems/gui_system.h>

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
    assert(provider.root_element != nullptr && "AHHHHHHHH");
}

void GUISystem::Update()
{
    GUISystemProvider &provider = GetProvider();
    if (provider.root_element == nullptr)
        return;
    provider.root_element->Rebuild(provider.context);
    provider.root_element->Update(provider.context);
    provider.gui_shader->Bind();
    provider.context->Draw();
}
} // namespace Vultr
