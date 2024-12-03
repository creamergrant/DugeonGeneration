#pragma once


namespace GameDev2D
{
    template <typename T>
    class MemoryBlock
    {
    public:
        //Creates to MemoryBlock object and storage in a single aligned allocation
        static MemoryBlock<T>* Create(unsigned int entriesPerBlock);

        /// Destroys the ObjectPoolBlock and associated storage.
        static void Destroy(MemoryBlock<T>* ptr);

        //Returns nullptr if there is no available space
        template <class... P>
        T* NewObject(P&&... params);

        //Deletes the allocation from the memory block
        void DeleteObject(const T* ptr);

        //Deletes all the allocations and reinitialise the block
        void DeleteAll();

        //Invokes the given function for all allocated entries
        template <typename F>
        void for_each(const F func) const;

        //Returns the number of object allocations
        unsigned int NumberOfAllocations() const;

        //Returns the max number of object allocations
        unsigned int MaxAllocations() const { return kEntriesPerBlock; }

    private:
        //Constructor and destructor are private, use the Create and Destroy static methods instead
        MemoryBlock(unsigned int entriesPerBlock);
        ~MemoryBlock();

        MemoryBlock(const MemoryBlock&) = delete;
        MemoryBlock& operator=(const MemoryBlock&) = delete;

        //Returns the start of indices of the memory block
        unsigned int* IndicesBegin() const;

        //Returns the starting address of the memory block
        T* MemoryBegin() const;

        //Member variables
        unsigned int m_FreeHeadIndex;
        const unsigned int kEntriesPerBlock;
    };
}

#include "MemoryBlock.inl"