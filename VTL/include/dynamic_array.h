#pragma once
#include <assert.h>
#include "types/types.hpp"
#include <memory>

namespace vtl
{
    template <typename T>
    struct DynamicArray
    {
        // Initialize an empty dynamic_array
        // Reserved specifies a size of the dynamic_array that will be reserved initially
        // which will be empty
        //
        // Returns new dynamic_array
        DynamicArray(uint reserved = 0)
        {
            if (reserved > 0)
            {
                internal_array = static_cast<T *>(malloc(reserved * sizeof(T)));
            }
        }

        DynamicArray(T *array, size_t count)
        {
            assert(count != 0 && "Count must be greater than 0!");
            assert(array != nullptr && "Array must not be null!");

            internal_array = static_cast<T *>(malloc(sizeof(T) * count));
            for (int i = 0; i < count; i++)
            {
                internal_array[i] = array[i];
            }
            len = count;
        }

        // Delete copy methods because we don't want this to be done on accident, we want it to be very very explicit since we are essentially duplicating a buffer
        DynamicArray<T> &operator=(const DynamicArray<T> &other) = delete;
        DynamicArray(const DynamicArray<T> &other) = delete;

        // Destructor for dynamic array
        ~DynamicArray()
        {
            if (internal_array != nullptr)
                free(internal_array);
        }

        // Push element to back of dynamic_array
        // If the dynamic_array does not have enough space, then a reallocation will
        // occur
        //
        // Returns the element just inserted
        T *push_back(const T &element)
        {
            // If there is not enough space in the dynamic_array, then reallocate
            if (len >= size)
            {
                reallocate(size + 1);
            }

            // Increase the len amount and assign the last element of array to the new
            // element
            internal_array[len++] = element;

            return &internal_array[len];
        }

        // Inserts element at specific index in dynamic_array
        // Shifts all elements at and to the right of that index 1 to the right
        // If the dynamic_array does not have enough space, then a reallocation will
        // occur
        //
        // Throws index out of bounds error if index is greater than the
        // dynamic_array.size or if index is negative
        //
        // Returns the element just inserted
        T *insert(size_t index, T element)
        {
            // Fail if the index is greater than the dynamic_array.size or negative
            assert(index <= size && index >= 0 && "Index out of bounds!");

            // If there is not enough space in the DynamicArray, then reallocate
            if (len >= size)
            {
                reallocate(size + 1);
            }

            if (size > 1)
            {
                // Shift all elements right
                for (int i = size - 2; i >= index; i--)
                {
                    internal_array[i + 1] = internal_array[i];
                }
            }

            // Increase the len amount
            len++;

            // Assign last element of array to new element
            internal_array[index] = element;

            return &internal_array[index];
        }

        // Delete element at index in dynamic_array
        // Shifts all elements to the right of that index 1 to the left
        // If the dynamic_array len equals dynamic_array size, then it will assume
        // nothing is reserved and a reallocation will occur unless reallocate is
        // explicitly false
        T remove(size_t index, bool reallocate = true)
        {
            // Fail if the index is greater than the dynamic_array.size or negative
            assert(index <= size && index >= 0 && "Index out of bounds!");

            // Save the element we are removing for later
            T element = internal_array[index];

            // Shift elements to the left
            for (uint i = index + 1; i < size; i++)
            {
                internal_array[i - 1] = internal_array[i];
            }

            // If the len == the size and reallocate is true, reallocate
            if (len == size && reallocate)
            {
                this->reallocate(size - 1);
            }

            // Decrease len even if no reallocation
            len--;

            return element;
        }

        static DynamicArray<T> copy()
        {
        }

        // Ability to index into the array and assign and retreive elements by
        // reference
        T &operator[](int index) const
        {
            assert(index < size && index >= 0 && "Index out of bounds");
            return internal_array[index];
        }

        // Spaces len (not bytes)
        size_t len;

        // Space available (not bytes)
        size_t size;

        // To make sure that the buffer expansion is geometric
        f64 growth_factor = 1.3;

      private:
        void reallocate(size_t new_size)
        {
            size = new_size;
            if (size == 0)
            {
                if (internal_array != nullptr)
                {
                    free(internal_array);
                    internal_array = nullptr;
                }
            }
            else
            {
                if (internal_array == nullptr)
                {
                    internal_array = (T *)malloc(new_size * sizeof(T));
                }
                else
                {
                    internal_array = (T *)realloc(internal_array, new_size * sizeof(T));
                }
            }
        }

        // The internal array
        T *internal_array;

        // Dumbass C++ shit
      public:
        struct Iterator
        {
            typedef std::random_access_iterator_tag IteratorCategory;
            typedef T ValueType;
            typedef ValueType *Pointer;
            typedef ValueType &Reference;

            Iterator(Pointer p_ptr) : ptr(p_ptr){};

            Iterator &operator++()
            {
                ptr++;
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator iterator = *this;
                ++(*this);
                return iterator;
            }

            Iterator &operator--()
            {
                ptr--;
                return *this;
            }

            Iterator operator--(int)
            {
                Iterator iterator = *this;
                --(*this);
                return iterator;
            }

            Reference operator[](int index)
            {
                return *(ptr + index);
            }

            Pointer operator->()
            {
                return ptr;
            }

            Reference operator*()
            {
                return *ptr;
            }

            bool operator==(const Iterator &other) const
            {
                return ptr == other.ptr;
            }

            bool operator!=(const Iterator &other) const
            {
                return !(*this == other);
            }

          private:
            Pointer ptr;
        };

        Iterator begin() const
        {
            return Iterator(internal_array);
        }

        Iterator end() const
        {
            return Iterator(internal_array + size);
        }
    };

} // namespace vtl
