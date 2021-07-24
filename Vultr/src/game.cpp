#include <game.hpp>

Scene *Game::perform_init_scene(Vultr::Engine *e, Vultr::World *world)
{
    if (this->scene != nullptr)
    {
        this->scene->flush(e);
        delete scene;
        this->scene = nullptr;
    }

    this->scene = init_scene(e, world);
    return this->scene;
}

void Game::update(Vultr::Engine *e, const Vultr::UpdateTick &tick)
{
    if (this->scene == nullptr)
        return;
    this->scene->update(e, tick);
}

void Game::flush_scene(Vultr::Engine *e)
{
    if (this->scene != nullptr)
    {
        this->scene->flush(e);
        delete this->scene;
        this->scene = nullptr;
    }
}

void Game::perform_flush(Vultr::Engine *e)
{
    flush(e);
    if (this->scene != nullptr)
    {
        this->scene->flush(e);
        delete this->scene;
        this->scene = nullptr;
    }
}
