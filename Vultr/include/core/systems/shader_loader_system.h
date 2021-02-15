#pragma once
#include <glad/glad.h>
#include <array>
#include <core/components/shader_cache.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <ecs/system/system.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>
#include <vector>

namespace Brick3D
{

class ShaderLoaderSystem : public System
{

  public:
    static void RegisterSystem();

  protected:
    void OnDestroyEntity(Entity entity) override;
    void OnCreateEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return ShaderLoaderSystemProvider::Get();
    }
};
} // namespace Brick3D
