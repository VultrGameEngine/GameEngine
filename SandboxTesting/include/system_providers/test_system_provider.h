#pragma once
#include <Vultr.hpp>

VSYSTEM()
class TestSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<TestSystemProvider> Get()
    {
        return Vultr::Engine::GetSystemProvider<TestSystemProvider>();
    }

    VPROPERTY()
    double test_prop = 0.0;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
