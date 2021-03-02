#include "core_components.h"
#include <cereal/archives/binary.hpp>
#include <ecs/component/component_array.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(ComponentArray<TransformComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(IComponentArray,
                                     ComponentArray<TransformComponent>);
CEREAL_REGISTER_TYPE(ComponentArray<StaticMeshComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(IComponentArray,
                                     ComponentArray<StaticMeshComponent>);
CEREAL_REGISTER_TYPE(ComponentArray<SkyBoxComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(IComponentArray,
                                     ComponentArray<SkyBoxComponent>);
CEREAL_REGISTER_TYPE(ComponentArray<MaterialComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(IComponentArray,
                                     ComponentArray<MaterialComponent>);
CEREAL_REGISTER_TYPE(ComponentArray<LightComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(IComponentArray,
                                     ComponentArray<LightComponent>);
CEREAL_REGISTER_TYPE(ComponentArray<ControllerComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(IComponentArray,
                                     ComponentArray<ControllerComponent>);
CEREAL_REGISTER_TYPE(ComponentArray<CameraComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(IComponentArray,
                                     ComponentArray<CameraComponent>);
