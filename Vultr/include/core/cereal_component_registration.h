#include "core_components.h"
#include <cereal/archives/binary.hpp>
#include <ecs/component/component_array.hpp>
#include <cereal/types/polymorphic.hpp>
#include <glm/glm.hpp>

template <class Archive> void serialize(Archive &archive, glm::vec3 &vec3)
{
    archive(vec3.x, vec3.y, vec3.z);
}
template <class Archive> void serialize(Archive &archive, glm::vec2 &vec2)
{
    archive(vec2.x, vec2.y);
}
template <class Archive> void serialize(Archive &archive, glm::quat &quat)
{
    archive(quat.x, quat.y, quat.z, quat.w);
}
