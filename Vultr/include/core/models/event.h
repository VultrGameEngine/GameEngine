#pragma once
#include <engine.hpp>

namespace Vultr
{

    struct EditEvent
    {
        virtual void Undo(Vultr::Engine *e) = 0;
        virtual void Redo(Vultr::Engine *e) = 0;
        virtual ~EditEvent() = default;
        ;
    };

    template <typename T>
    struct ComponentEditEvent : EditEvent
    {
        std::set<Vultr::Entity> entities;
        Vultr::ComponentType type;
        T before;
        T after;

        void Undo(Vultr::Engine *e) override
        {
            for (Entity entity : entities)
            {
                if (!entity_has_component<T>(e, entity))
                {
                    fprintf(stderr, "Failed to undo!");
                    return;
                }

                entity_get_component<T>(e, entity) = before;
            }
        }

        void Redo(Vultr::Engine *e) override
        {
            for (Entity entity : entities)
            {
                if (!entity_has_component<T>(e, entity))
                {
                    fprintf(stderr, "Failed to redo!");
                    return;
                }

                entity_get_component<T>(e, entity) = after;
            }
        }
    };

    struct EntityDestroyEvent : EditEvent
    {
        Vultr::Entity entity;
        std::unordered_map<Vultr::ComponentType, void *> components;

        void Undo(Vultr::Engine *e) override
        {
            entity = create_entity(get_current_world(e));
            auto *world = get_current_world(e);
            auto &component_manager = world_get_component_manager(world);
            Signature signature;
            for (auto [type, component] : components)
            {
                component_manager.component_arrays[type]->InsertData(entity, component);
                signature.set(type);
            }
            system_manager_entity_signature_changed(e, world_get_system_manager(world), entity, signature);

            system_manager_entity_signature_changed(e, e->system_manager, entity, signature);
            entity_manager_set_signature(world_get_entity_manager(world), entity, signature);
        }

        void Redo(Vultr::Engine *e) override
        {
            destroy_entity(e, entity);
        }

        ~EntityDestroyEvent()
        {
            for (auto [type, component] : components)
            {
                if (component != nullptr)
                    free(component);
            }
        }
    };

} // namespace Vultr
