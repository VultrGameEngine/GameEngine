#pragma once
#include <vultr.hpp>

struct EditEventManager
{
    // Undo/redo layer, index into the event stack
    s32 event_index = -1;

    // Holds all edit events
    std::vector<Vultr::EditEvent *> event_stack;
};
