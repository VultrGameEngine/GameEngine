#include <sandbox_test.h>
#include <components/test_component.h>
#include <systems/test_system.h>
#include <component_registrar.h>

using namespace Vultr;
void SandboxTest::RegisterComponents()
{
    register_components();
}

void SandboxTest::Init()
{
    TestSystem::register_system();
}

void SandboxTest::Update(const Vultr::UpdateTick &tick)
{
    TestSystem::update(tick);
    printf("%f ms\n", tick.m_delta_time);
}

void SandboxTest::Flush()
{
}

Game *init(Vultr::Engine *engine)
{
    return new SandboxTest(engine);
}
void flush(Game *game)
{
    delete game;
}
