#pragma once
#include <types/types.hpp>
#include <thread.h>

namespace vtl
{
    template <typename T>
    struct Queue
    {
        T *items = nullptr;
        size_t len = 0;
        vtl::mutex queue_mutex;
        vtl::condition_variable queue_cond;

        Queue()
        {
        }

        ~Queue()
        {
            if (items != nullptr)
            {
                free(items);
            }
        }

        Queue(const Queue &) = delete;
        void operator=(const Queue &) = delete;

        T *front()
        {
            std::unique_lock<vtl::mutex> lock(queue_mutex);
            assert(len > 0 && "No items in queue!");
            T *item = &items[len - 1];
            return item;
        }

        void push(T *item)
        {
            std::unique_lock<vtl::mutex> lock(queue_mutex);
            if (len == 0)
            {
                assert(items == nullptr && "Items already allocated for some reason?");
                len++;
                items = static_cast<T *>(malloc(sizeof(T)));
            }
            else
            {
                len++;
                resize();
                for (s32 i = 0; i < len - 1; i++)
                {
                    items[i + 1] = items[i];
                }
            }

            items[0] = *item;
            lock.unlock();
            queue_cond.notify_one();
        }

        T pop()
        {
            std::unique_lock<vtl::mutex> lock(queue_mutex);
            while (empty())
            {
                queue_cond.wait(lock);
            }

            T copy = items[len - 1];
            if (len == 1)
            {
                len--;
                free(items);
                items = nullptr;
            }
            else
            {
                len--;
                resize();
            }

            return copy;
        }

        void resize()
        {
            items = static_cast<T *>(realloc(items, sizeof(T) * len));
        }

        bool empty() const
        {
            return len == 0;
        }
    };
} // namespace vtl
