#pragma once
#include <glm/gtx/quaternion.hpp>

namespace nlohmann
{
    template <>
    struct adl_serializer<glm::vec3>
    {
        static void to_json(json &j, const glm::vec3 &v)
        {
            j["x"] = v.x;
            j["y"] = v.y;
            j["z"] = v.z;
        }

        static void from_json(const json &j, glm::vec3 &v)
        {
            v.x = j["x"].get<float>();
            v.y = j["y"].get<float>();
            v.z = j["z"].get<float>();
        }
    };

    template <>
    struct adl_serializer<glm::vec4>
    {
        static void to_json(json &j, const glm::vec4 &v)
        {
            j["x"] = v.x;
            j["y"] = v.y;
            j["z"] = v.z;
            j["w"] = v.w;
        }

        static void from_json(const json &j, glm::vec4 &v)
        {
            v.x = j["x"].get<float>();
            v.y = j["y"].get<float>();
            v.z = j["z"].get<float>();
            v.w = j["w"].get<float>();
        }
    };

    template <>
    struct adl_serializer<glm::quat>
    {
        static void to_json(json &j, const glm::quat &v)
        {
            j["x"] = v.x;
            j["y"] = v.y;
            j["z"] = v.z;
            j["w"] = v.w;
        }

        static void from_json(const json &j, glm::quat &v)
        {
            v.x = j["x"].get<float>();
            v.y = j["y"].get<float>();
            v.z = j["z"].get<float>();
            v.w = j["w"].get<float>();
        }
    };
} // namespace nlohmann
