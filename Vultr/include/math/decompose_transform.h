#pragma once
#include <glm/glm.hpp>

namespace Vultr
{
namespace Math
{

bool DecomposeTransform(const glm::mat4 &transform, glm::vec3 &translation,
                        glm::vec3 &rotation, glm::vec3 &scale);
}
} // namespace Vultr
