#pragma once
#include "ui_id.h"
#include <types/types.hpp>

namespace Vultr
{
    namespace IMGUI
    {

        typedef u32 WidgetType;
#define MAX_WIDGETS 1000
        struct IWidgetCache
        {
            virtual void widget_destroyed(UI_ID id){};
            virtual bool has_data(UI_ID id)
            {
                return false;
            }
            virtual ~IWidgetCache() = default;
        };

        template <typename T>
        struct WidgetCache : IWidgetCache
        {
            void insert_data(UI_ID id)
            {
                assert(widget_to_index_map.find(id) == widget_to_index_map.end() && "Cache added to same widget more than once");

                // Create a new index
                // This is set to size because that is the logical next index
                size_t new_index = size;

                // Create the relations between the entity ID and the new_index in the
                // component array
                widget_to_index_map[id] = new_index;
                index_to_widget_map[new_index] = id;

                // Set the component in the component_array
                cache_array[new_index] = T{};

                ++size;
            }

            void remove_data(UI_ID id)
            {
                assert(widget_to_index_map.find(id) != widget_to_index_map.end() && "Attempting to remove nonexistent widget cache");

                // Get the index for the specified entity ID
                size_t index_of_removed_widget = widget_to_index_map[id];

                // Get the index of the last element of the component array
                size_t index_of_last_element = size - 1;

                // Move the last element of the component array into the removed entity's
                // index
                cache_array[index_of_removed_widget] = cache_array[index_of_last_element];

                // Update the maps for the newly moved element
                UI_ID entity_of_last_element = widget_to_index_map[index_of_last_element];
                widget_to_index_map[entity_of_last_element] = index_of_removed_widget;
                index_to_widget_map[index_of_removed_widget] = entity_of_last_element;

                // Remove the entity requested from the maps
                widget_to_index_map.erase(id);
                index_to_widget_map.erase(index_of_last_element);

                --size;
            }

            void widget_destroyed(UI_ID id) override
            {
                if (widget_to_index_map.find(id) != widget_to_index_map.end())
                {
                    remove_data(id);
                }
            }

            bool has_data(UI_ID id) override
            {
                return widget_to_index_map.find(id) != widget_to_index_map.end();
            }

            T &get_data(UI_ID id)
            {
                assert(widget_to_index_map.find(id) != widget_to_index_map.end() && "Attempting to retreive data from nonexistent widget cache");

                return cache_array[widget_to_index_map[id]];
            }

            std::array<T, MAX_WIDGETS> cache_array{};

            std::unordered_map<UI_ID, size_t> widget_to_index_map{};
            std::unordered_map<size_t, UI_ID> index_to_widget_map{};

            size_t size{};
        };
    } // namespace IMGUI
} // namespace Vultr
