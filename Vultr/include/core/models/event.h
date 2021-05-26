#pragma once
#include <engine.hpp>

namespace Vultr
{

    struct EditEvent
    {
        virtual void Undo(Vultr::Engine *e) = 0;
        virtual void Redo(Vultr::Engine *e) = 0;
    };

    template <typename T>
    struct ComponentEditEvent : EditEvent
    {
        std::set<Vultr::Entity> entities;
        Vultr::ComponentType type;
        T before;
        T after;

        void Undo(Vultr::Engine *engine) override
        {
            for (Entity e : entities)
            {
                if (!entity_has_component<T>(e))
                {
                    std::cout << "Failed to undo!" << std::endl;
                    return;
                }

                entity_get_component<T>(e) = before;
            }
        }

        void Redo(Vultr::Engine *engine) override
        {
            for (Entity e : entities)
            {
                if (!entity_has_component<T>(e))
                {
                    std::cout << "Failed to redo!" << std::endl;
                    return;
                }

                entity_get_component<T>(e) = after;
            }
        }
    };

} // namespace Vultr
