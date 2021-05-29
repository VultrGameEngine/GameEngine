#pragma once
#include <core/core_components.h>
#include <json/json_fwd.hpp>

namespace Vultr
{
    void to_json(json &j, const CameraComponent &c);
    void from_json(const json &j, CameraComponent &c);
    void to_json(json &j, const ControllerComponent &c);
    void from_json(const json &j, ControllerComponent &c);
    void to_json(json &j, const LightComponent &c);
    void from_json(const json &j, LightComponent &c);
    void to_json(json &j, const SkyBoxComponent &c);
    void from_json(const json &j, SkyBoxComponent &c);
    void to_json(json &j, const StaticMeshComponent &c);
    void from_json(const json &j, StaticMeshComponent &c);
    void to_json(json &j, const TransformComponent &c);
    void from_json(const json &j, TransformComponent &c);

    void to_json(json &j, const MaterialComponent &c);
    void from_json(const json &j, MaterialComponent &c);
    void to_json(json &j, const MaterialComponent::TexturePair &c);
    void from_json(const json &j, MaterialComponent::TexturePair &c);
    void to_json(json &j, const Color &c);
    void from_json(const json &j, Color &c);
} // namespace Vultr
