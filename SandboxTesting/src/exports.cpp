#include <sandbox_test.h>
#include <exports.h>
void *init(void *engine)
{
    return new SandboxTest((Vultr::Engine *)engine);
}
void flush(void *game)
{
    delete game;
}
