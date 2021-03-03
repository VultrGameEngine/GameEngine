#pragma once
#include <core/components/material_component.h>

namespace Vultr
{
namespace PBRMaterial
{
MaterialComponent Create(const std::string &p_diffuse_path,
                         const std::string &p_specular_path);

} // namespace PBRMaterial

} // namespace Vultr
