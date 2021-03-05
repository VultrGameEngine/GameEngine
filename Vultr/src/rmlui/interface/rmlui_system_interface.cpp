#include <rmlui/interface/rmlui_system_interface.h>
#include <Vultr.hpp>

namespace Vultr
{
RmlUiInterface::RmlUiInterface(Engine *p_engine) : engine(p_engine)
{
}
double RmlUiInterface::GetElapsedTime()
{
    return this->engine->GetElapsedTime();
}
} // namespace Vultr
