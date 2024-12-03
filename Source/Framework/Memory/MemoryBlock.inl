#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <limits>
#include <memory>

const unsigned int MIN_BLOCK_ALIGN = 64;


//Aligns n to align. n will be unchanged if it is already aligned
inline size_t align_to(size_t n, size_t align)
{
    return (1 + (n - 1) / align) * align;
}

namespace GameDev2D
{
    template <typename T>
    MemoryBlock<T>* MemoryBlock<T>::Create(unsigned int entriesPerBlock)
    {
        const size_t headerSize = sizeof(MemoryBlock<T>);
        const size_t indicesSize = align_to(sizeof(unsigned int) * entriesPerBlock, sizeof(unsigned int));
        const size_t entriesSize = sizeof(T) * entriesPerBlock;
        const size_t blockSize = headerSize + indicesSize + entriesSize;

        MemoryBlock<T>* ptr = reinterpret_cast<MemoryBlock<T>*>(_aligned_malloc(blockSize, MIN_BLOCK_ALIGN));
        if (ptr)
        {
            new (ptr) MemoryBlock(entriesPerBlock);
            assert(reinterpret_cast<uint8_t*>(ptr->IndicesBegin())
                == reinterpret_cast<uint8_t*>(ptr) + headerSize);
            assert(reinterpret_cast<uint8_t*>(ptr->MemoryBegin())
                == reinterpret_cast<uint8_t*>(ptr) + headerSize + indicesSize);
        }

        return ptr;
    }

    template <typename T>
    void MemoryBlock<T>::Destroy(MemoryBlock<T>* ptr)
    {
        ptr->~MemoryBlock();
        _aligned_free(ptr);
    }

    template <typename T>
    MemoryBlock<T>::MemoryBlock(unsigned int entriesPerBlock)
        : m_FreeHeadIndex(0), kEntriesPerBlock(entriesPerBlock)
    {
        unsigned int* indices = IndicesBegin();
        for (unsigned int i = 0; i < kEntriesPerBlock; ++i)
        {
            indices[i] = i + 1;
        }
    }

    template <typename T>
    MemoryBlock<T>::~MemoryBlock()
    {
        DeleteAll();
    }

    template <typename T>
    unsigned int* MemoryBlock<T>::IndicesBegin() const
    {
        // calculcates the start of the indicies
        return reinterpret_cast<unsigned int*>(const_cast<MemoryBlock<T>*>(this + 1));
    }

    template <typename T>
    T* MemoryBlock<T>::MemoryBegin() const
    {
        // calculates the start of pool memory
        return reinterpret_cast<T*>(IndicesBegin() + kEntriesPerBlock);
    }

    template <typename T>
    template <class... P>
    T* MemoryBlock<T>::NewObject(P&&... params)
    {
        //Get the head of the free list
        const unsigned int index = m_FreeHeadIndex;
        if (index != kEntriesPerBlock)
        {
            unsigned int* indices = IndicesBegin();
            assert(indices[index] != index); // assert that this index is not in use

            // update head of the free list
            m_FreeHeadIndex = indices[index];

            // flag index as used by assigning it's own index
            indices[index] = index;

            // get object memory
            T* ptr = MemoryBegin() + index;

            // construct the entry
            new (ptr) T(std::forward<P>(params)...);
            return ptr;
        }

        //Returns nullptr if there is no available space
        return nullptr;
    }

    template <typename T>
    void MemoryBlock<T>::DeleteObject(const T* ptr)
    {
        if (ptr)
        {
            const T* begin = MemoryBegin();
            assert(ptr >= begin && ptr < (begin + kEntriesPerBlock)); // assert that pointer is in range

            //Invoke the destructor
            ptr->~T();

            //Get the index of this pointer
            const unsigned int index = static_cast<unsigned int>(ptr - begin);
            unsigned int* indices = IndicesBegin();
            assert(indices[index] == index); // assert this index is allocated

            //Remove index from used list
            indices[index] = m_FreeHeadIndex;
            m_FreeHeadIndex = index;
        }
    }

    template <typename T>
    void MemoryBlock<T>::DeleteAll()
    {
        if (!std::is_trivially_destructible<T>::value)
        {
            const unsigned int* indices = IndicesBegin();
            T* first = MemoryBegin();
            for (unsigned int i = 0, count = kEntriesPerBlock; i != count; ++i)
            {
                if (indices[i] == i)
                {
                    T* ptr = first + i;
                    ptr->~T();
                }
            }
        }

        m_FreeHeadIndex = 0;
        unsigned int* indices = IndicesBegin();
        for (unsigned int i = 0; i < kEntriesPerBlock; ++i)
        {
            indices[i] = i + 1;
        }
    }


    template <typename T>
    template <typename F>
    void MemoryBlock<T>::for_each(const F func) const
    {
        const unsigned int* indices = IndicesBegin();
        T* first = MemoryBegin();
        for (unsigned int i = 0, count = kEntriesPerBlock; i != count; ++i)
        {
            if (indices[i] == i)
            {
                func(first + i);
            }
        }
    }

    template <typename T>
    unsigned int MemoryBlock<T>::NumberOfAllocations() const
    {
        unsigned int numberOfAllocations = 0;
        for_each([&numberOfAllocations](const T*)
        {
            ++numberOfAllocations;
        });
        return numberOfAllocations;
    }
}