#pragma once
#include <glad/glad.h>
#include <array>
#include <core/system_providers/shader_loader_system_provider.h>
#include <ecs/system/system.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>
#include <vector>

namespace Vultr
{

class ShaderLoaderSystem : public System
{

  public:
    static void RegisterSystem();
    static void Update();

  protected:
    static void CheckAndLoadShader(Entity entity);

    void OnDestroyEntity(Entity entity) override;
    void OnCreateEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return ShaderLoaderSystemProvider::Get();
    }
};
} // namespace Vultr
