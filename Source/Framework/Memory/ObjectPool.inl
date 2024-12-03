namespace GameDev2D
{
    template <typename T>
    ObjectPool<T>::ObjectPool(unsigned int maxEntries)
        : m_Block(MemoryBlock<T>::Create(maxEntries))
    {}

    template <typename T>
    ObjectPool<T>::~ObjectPool()
    {
        MemoryBlock<T>::Destroy(m_Block);
    }

    template <typename T>
    template <class... P>
    T* ObjectPool<T>::CreateObject(P&&... params)
    {
        return m_Block->NewObject(std::forward<P>(params)...);
    }

    template <typename T>
    void ObjectPool<T>::DestroyObject(const T* ptr)
    {
        m_Block->DeleteObject(ptr);
    }

    template <typename T>
    void ObjectPool<T>::DestroyAll()
    {
        m_Block->DeleteAll();
    }

    template <typename T>
    template <typename F>
    void ObjectPool<T>::for_each(const F func) const
    {
        m_Block->for_each(func);
    }
}