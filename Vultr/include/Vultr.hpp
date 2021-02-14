#pragma once
#include "Game.hpp"
#include <core/core.h>

class Vultr
{
  public:
    void Init(bool debug);
    void LoadGame(const std::string &path);
    void InitGame();
    void UpdateGame(const Brick3D::UpdateTick &tick);
    void Destroy();

  private:
    GLFWwindow *window;
    bool debug;
    Game *game;
};
