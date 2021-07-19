#pragma once
#include <types/types.hpp>
#include <gui/core/context.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct ListState
        {
        };

        struct ListContext
        {
            u32 index;
        };

        struct ListIterator
        {
            typedef std::forward_iterator_tag IteratorCategory;
            typedef ListContext ValueType;
            typedef ValueType *Pointer;
            typedef ValueType &Reference;
            ListIterator(Pointer p_ptr) : ptr(p_ptr)
            {
            }

            ListIterator &operator++()
            {
                do
                {
                    ptr++;
                } while (ptr == nullptr);
                return *this;
            }

            ListIterator operator++(int)
            {
                ListIterator iterator = *this;
                ++(*this);
                return iterator;
            }

            ListIterator &operator--()
            {
                do
                {
                    ptr--;
                } while (ptr == nullptr);
                return *this;
            }

            ListIterator operator--(int)
            {
                ListIterator iterator = *this;
                --(*this);
                return iterator;
            }

            Pointer operator->()
            {
                while (ptr == nullptr)
                {
                    (*this)++;
                }
                return ptr;
            }

            Reference operator*()
            {
                while (ptr == nullptr)
                {
                    (*this)++;
                }
                return *ptr;
            }

            bool operator==(const ListIterator &other) const
            {
                return ptr == other.ptr;
            }

            bool operator!=(const ListIterator &other) const
            {
                return !(*this == other);
            }

          private:
            Pointer ptr;
        };

        struct ListBuilder
        {
            ListIterator begin();
            ListIterator end();
        };

        struct ListViewStyle
        {
        };

        struct ListViewState
        {
            ListViewStyle style = {};
        };

        ListBuilder listview_builder();

        void begin_listview(Context *c, UI_ID id, ListViewStyle = {});
        void end_listview(Context *c);
    } // namespace IMGUI
} // namespace Vultr
