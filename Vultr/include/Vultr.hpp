#pragma once
#include "Game.hpp"
#include <core/core.h>
#include <rmlui/interface/rmlui_system_interface.h>

namespace Vultr
{
class Engine
{
  public:
    void Init(bool debug);
    void LoadGame(const std::string &path);
    void LoadGame(Game *game);
    static void RegisterComponents();
    void InitSystems();
    void InitGame();
    void UpdateGame(float &t);
    void Destroy();
    double GetElapsedTime();
    GLFWwindow *window;
    bool should_close = false;

  private:
    bool debug;
    Game *game;
    RmlUiInterface *ui_interface;
};
} // namespace Vultr
