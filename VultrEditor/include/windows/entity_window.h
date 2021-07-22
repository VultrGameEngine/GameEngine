#pragma once
#include <editor.h>

void register_entity_window(Vultr::Engine *e, Editor *editor);
void entity_window_render(Vultr::Engine *e, Editor *editor, const Vultr::UpdateTick &tick, void *state);
