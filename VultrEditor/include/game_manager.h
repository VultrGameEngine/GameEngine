#pragma once
#include <vultr.hpp>

struct GameManager
{
    // If playing is true, then the game is actively being updated each frame, otherwise it means that the game is either paused or hasn't been started
    bool playing = false;

    // If game_running is true, then the game has been started, otherwise the game is stopped
    bool game_running = false;

    // Holds a copy of the world that is created when the user presses the play button. An Instance of the world is copied and stored in memory that will then be restored when the game is stopped
    Vultr::World *cached_world = nullptr;
};
