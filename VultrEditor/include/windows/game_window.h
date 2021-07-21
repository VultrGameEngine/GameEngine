#pragma once
#include <editor.h>

void register_game_window(Vultr::Engine *e);
void game_window_render(Vultr::Engine *e, const Vultr::UpdateTick &tick, void *state);
