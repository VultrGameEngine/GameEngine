#pragma once
#include <assert.h>
#include "types/types.hpp"
#include <memory>

namespace vtl
{
    template <typename T>
    struct DynamicArray
    {
        // Destructor for dynamic array
        ~DynamicArray()
        {
            if (internal_array != nullptr)
                free(internal_array);
        }

        // Ability to index into the array and assign and retreive elements by
        // reference
        T &operator[](int index) const
        {
            assert(index < size && index >= 0 && "Index out of bounds");
            return internal_array[index];
        }

        struct Iterator
        {
            typedef std::random_access_iterator_tag IteratorCategory;
            typedef T ValueType;
            typedef ValueType *Pointer;
            typedef ValueType &Reference;
            Iterator(Pointer p_ptr) : ptr(p_ptr)
            {
            }

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

        // The internal array
        T *internal_array;

        // Spaces used (not bytes)
        uint used;
        // Space available (not bytes)
        uint size;
    };

    namespace internal
    {
        template <typename T>
        static void dynamic_array_realloc(DynamicArray<T> &a, const uint new_size)
        {
            a.size = new_size;
            if (a.size == 0)
            {
                if (a.internal_array != nullptr)
                {
                    free(a.internal_array);
                    a.internal_array = nullptr;
                }
            }
            else
            {
                if (a.internal_array == nullptr)
                {
                    a.internal_array = (T *)malloc(new_size * sizeof(T));
                }
                else
                {
                    a.internal_array = (T *)realloc(a.internal_array, new_size * sizeof(T));
                }
            }
        }
    } // namespace internal

    // Initialize an empty dynamic_array
    // Reserved specifies a size of the dynamic_array that will be reserved initially
    // which will be empty
    //
    // Returns new dynamic_array
    template <typename T>
    DynamicArray<T> new_dynamic_array(uint reserved = 0)
    {
        T *arr = nullptr;
        if (reserved > 0)
        {
            arr = (T *)malloc(reserved * sizeof(T));
        }
        DynamicArray<T> list = {arr, 0, reserved};
        return list;
    }

    // Initialize an dynamic_array with another array
    // Count is the number of elements in the array
    //
    // Returns new dynamic_array
    template <typename T>
    DynamicArray<T> new_dynamic_array(T *array, uint count)
    {
        DynamicArray<T> a = new_dynamic_array<T>(count);
        for (int i = 0; i < count; i++)
        {
            a.internal_array[i] = array[i];
        }
        a.used = count;
        return a;
    }

    // Push element to back of dynamic_array
    // If the dynamic_array does not have enough space, then a reallocation will
    // occur
    //
    // Returns the element just inserted
    template <typename T>
    T dynamic_array_push_back(DynamicArray<T> &a, T element)
    {
        // If there is not enough space in the dynamic_array, then reallocate
        if (a.used >= a.size)
        {
            internal::dynamic_array_realloc(a, a.size + 1);
        }

        // Increase the used amount and assign the last element of array to the new
        // element
        a.internal_array[a.used++] = element;

        return element;
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
    template <typename T>
    T dynamic_array_insert(DynamicArray<T> &a, int index, T element)
    {
        // Fail if the index is greater than the dynamic_array.size or negative
        assert(index <= a.size && index >= 0 && "Index out of bounds!");

        // If there is not enough space in the DynamicArray, then reallocate
        if (a.used >= a.size)
        {
            internal::dynamic_array_realloc(a, a.size + 1);
        }

        if (a.size > 1)
        {
            // Shift all elements right
            for (int i = a.size - 2; i >= index; i--)
            {
                a.internal_array[i + 1] = a.internal_array[i];
            }
        }

        // Increase the used amount
        a.used++;

        // Assign last element of array to new element
        a.internal_array[index] = element;

        return element;
    }

    // Delete element at index in dynamic_array
    // Shifts all elements to the right of that index 1 to the left
    // If the dynamic_array used equals dynamic_array size, then it will assume
    // nothing is reserved and a reallocation will occur unless reallocate is
    // explicitly false
    template <typename T>
    T dynamic_array_delete_element(DynamicArray<T> &a, uint index, bool reallocate = true)
    {
        // Fail if the index is greater than the dynamic_array.size or negative
        assert(index <= a.size && index >= 0 && "Index out of bounds!");

        // Save the element we are removing for later
        T element = a[index];

        // Shift elements to the left
        for (uint i = index + 1; i < a.size; i++)
        {
            a.internal_array[i - 1] = a.internal_array[i];
        }

        // If the used == the size and reallocate is true, reallocate
        if (a.used == a.size && reallocate)
        {
            internal::dynamic_array_realloc(a, a.size - 1);
        }

        // Decrease used even if no reallocation
        a.used--;

        return element;
    }

    // Get the size of the array
    // NOT the number of elements
    template <typename T>
    uint sizeof_dynamic_array(const DynamicArray<T> &a)
    {
        return a.size;
    }

} // namespace vtl
