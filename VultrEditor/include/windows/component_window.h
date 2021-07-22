#pragma once
#include <editor.h>

void register_component_window(Vultr::Engine *e, Editor *editor);
void component_window_render(Vultr::Engine *e, Editor *editor, const Vultr::UpdateTick &tick, void *state);
