#pragma once
#include <Vultr.hpp>
#include <system_providers/test_system_provider.h>

class TestSystem
{
  public:
    static void RegisterSystem();

  protected:
    TestSystemProvider &GetProvider()
    {
        return *(TestSystemProvider::Get());
    }

  private:
    static void OnCreateEntity(Entity entity);
    friend TestSystemProvider;
};
