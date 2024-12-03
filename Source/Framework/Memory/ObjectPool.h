#pragma once

#include "MemoryBlock.h"


namespace GameDev2D
{
    template <typename T>
    class ObjectPool
    {
    public:
        ObjectPool(unsigned int maxEntries);
        ~ObjectPool();

        //Creates a new object from the ObjectPool
        template <class... P>
        T* CreateObject(P&&... params);

        //Destroys the object from the ObjectPool
        void DestroyObject(const T* ptr);

        //Destroys all the current objects in the ObjectPool
        void DestroyAll();

        //Invokes the given function for all allocated entries
        template <typename F>
        void for_each(const F func) const;

        //Returns the number of used allocations
        unsigned int NumberOfUsedAllocations() const { return m_Block->NumberOfAllocations(); }

        //Returns the number of available allocations
        unsigned int NumberOfAvailableAllocations() const { return m_Block->MaxAllocations() - m_Block->NumberOfAllocations(); }

        //Returns the max number of allocations
        unsigned int MaxNumberOfAllocations() const { return m_Block->MaxAllocations(); }

    private:
        MemoryBlock<T>* m_Block;

        ObjectPool(const ObjectPool&) = delete;
        ObjectPool& operator=(const ObjectPool&) = delete;
    };
}

#include "ObjectPool.inl"