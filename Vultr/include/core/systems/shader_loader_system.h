#pragma once
#include <glad/glad.h>
#include <array>
#include <core/system_providers/shader_loader_system_provider.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>
#include <vector>
#include <core/components/material_component.h>

namespace Vultr
{

class ShaderLoaderSystem
{

  public:
    static void LoadShader(const MaterialComponent &mat);
    static void RegisterSystem();
    static void Update();

    static void OnDestroyEntity(Entity entity);
    static void OnCreateEntity(Entity entity);
    static SystemProvider &GetProvider()
    {
        std::shared_ptr<ShaderLoaderSystemProvider> provider =
            ShaderLoaderSystemProvider::Get();
        return *provider;
    }

  protected:
    static void CheckAndLoadShader(Entity entity);
};
} // namespace Vultr
