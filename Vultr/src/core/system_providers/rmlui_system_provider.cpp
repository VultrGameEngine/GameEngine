#include <core/system_providers/rmlui_system_provider.h>
#include <helpers/path.h>

namespace Vultr
{
void RmlUiSystemProvider::LoadDocument(const std::string &path)
{
    Get()->document = Get()->context->LoadDocument(Path::GetFullPath(path));
    if (Get()->document)
        Get()->document->Show();
    Get()->context->Update();
}

void RmlUiSystemProvider::LoadFont(const std::string &path)
{
    bool result = Rml::LoadFontFace(Path::GetFullPath(path));
}

void RmlUiSystemProvider::OnCreateEntity(Entity entity)
{
}

void RmlUiSystemProvider::OnDestroyEntity(Entity entity)
{
}
} // namespace Vultr
