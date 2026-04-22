#include <iterator> 
#include <cstddef> 

template <typename T>
class Iterator 
{
    private:
    std::size_t capacity;
    std::size_t head;
    T* data;
    std::size_t logical_pos;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T&; 
    using pointer = T*; 

    Iterator(T* d, std::size_t c, std::size_t h, std::size_t pos) 
        : data(d), capacity(c), head(h), logical_pos(pos) {}


    operator Iterator<const T>() const 
    {
        return Iterator<const T>(data, capacity, head, logical_pos);
    }

    reference operator*() const
    {
        return data[(head + logical_pos) % capacity];
    }

    pointer operator->() const
    {
        return &data[(head + logical_pos) % capacity];
    }
    reference operator[](difference_type n) const
    {
        return data[(head + logical_pos + n) % capacity];
    }

    Iterator& operator++() 
    {
        ++logical_pos; 
        return *this;
    }
    Iterator operator++(int) 
    {
        Iterator old = *this; 
        ++logical_pos;
        return old;
    }
    template <typename U>
    bool operator!=(const Iterator<U>& other) const
    {
        return !(*this == other);
    }
    Iterator& operator--()
    {
        --logical_pos;
        return *this;
    }
    Iterator operator--(int)
    {
        Iterator old = *this; 
        --logical_pos;
        return old;

    }
    Iterator& operator+=(difference_type n)
    {
        logical_pos += n;
        return *this;
    }
    Iterator operator+(difference_type n) const
    {
        return Iterator(data, capacity, head, logical_pos + n);
    }
    friend Iterator operator+(difference_type n, Iterator a)
    {
        return a + n;
    }

    Iterator& operator-=(difference_type n) 
    {
        logical_pos -= n;
        return *this;
    }
    Iterator operator-(difference_type n) const
    {
        return Iterator(data, capacity, head, logical_pos - n);
    }
    friend difference_type operator-(const Iterator a, const Iterator b)
    {
       return (a.logical_pos) - b.logical_pos;
    }
    template <typename U>
    bool operator<(const Iterator<U>& other) const
    {
        return logical_pos < other.logical_pos;
    }
    template <typename U>
    bool operator>(const Iterator<U>& other) const
    {
        return logical_pos > other.logical_pos;
    }
    template <typename U>
    bool operator>=(const Iterator<U>& other) const
    {
        return logical_pos >= other.logical_pos;
    }

    template <typename U>
    bool operator<=(const Iterator<U>& other) const
    {
        return logical_pos <= other.logical_pos;
    }
    template <typename U>
    bool operator==(const Iterator<U>& other) const
    {
        return (logical_pos == other.logical_pos) && (data == other.data);
    }
    pointer get()
    {
        return &data[(head + logical_pos) % capacity];
    }
    const T* get() const
    {
        return &data[(head + logical_pos) % capacity];
    }
    
};