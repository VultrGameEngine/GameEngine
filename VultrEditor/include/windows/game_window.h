#pragma once
#include <editor.h>

void register_game_window(Vultr::Engine *e, Editor *editor);
void game_window_render(Vultr::Engine *e, Editor *editor, const Vultr::UpdateTick &tick, void *state);
