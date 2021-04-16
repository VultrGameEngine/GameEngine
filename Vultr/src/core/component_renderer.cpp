#include <core/component_renderer.h>

template <> inline void RenderMember(const std::string &name, std::string &m)
{
    ImGui::InputText(name.c_str(), &m[0], sizeof(char) * 4096);
}

template <> inline void RenderMember(const std::string &name, float &m)
{
    ImGui::DragFloat(name.c_str(), &m, 0.02f);
}

template <> inline void RenderMember(const std::string &name, int &m)
{
    ImGui::DragInt(name.c_str(), &m);
}

template <> inline void RenderMember(const std::string &name, double &m)
{
    ImGui::DragFloat(name.c_str(), (float *)&m, 0.02f);
}

template <> inline void RenderMember(const std::string &name, bool &m)
{
    ImGui::Checkbox(name.c_str(), &m);
}

template <> inline void RenderMember(const std::string &name, glm::vec3 &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
}

template <> inline void RenderMember(const std::string &name, glm::quat &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
    ImGui::DragFloat((name + ".w").c_str(), &m.w, 0.02f);
}
#ifndef WIN32
VULTR_REGISTER_COMPONENT(TransformComponent, position, rotation, scale);
VULTR_REGISTER_COMPONENT(StaticMeshComponent, m_path);
VULTR_REGISTER_COMPONENT(SkyBoxComponent, identifier, front, back, top, bottom, left,
                         right);
// REGISTER_COMPONENT_CEREAL(MaterialComponent)
VULTR_REGISTER_COMPONENT(LightComponent, some_param);
VULTR_REGISTER_COMPONENT(ControllerComponent, sens);
VULTR_REGISTER_COMPONENT(CameraComponent, enabled, fov, znear, zfar);
#endif
