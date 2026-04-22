#include <iostream>
#include<memory>

#include "reverse_iterator.h"

template<typename T, bool IsExtendable = false, typename Allocator = std::allocator<T>>
class circular_buffer 
{
private:
    std::size_t capacity;
    std::size_t m_size;
    std::size_t head;
    std::size_t tail;
    Allocator allocator;
    T* data;
public:

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<T>;
    using const_iterator = const Iterator<const T>;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using reverse_iterator = ReverseIterator<T>;
    using const_reverse_iterator = ReverseIterator<const T>;
    using allocator_type = Allocator;

    circular_buffer():
        capacity(0),
        m_size(0),
        head(0),
        tail(0),
        data(nullptr),
        allocator(Allocator()) {}
    circular_buffer(std::size_t capacity_):
        capacity(capacity_),
        m_size(0), head(0), tail(0)
        {
            allocator = Allocator();
            data = allocator.allocate(capacity);
        }
    circular_buffer(std::size_t count, const T& val)
    :   circular_buffer(count)
    {
        m_size = count;
        tail = m_size;
        for(std::size_t i = 0; i < count; ++i)
        {
            new (&data[i]) T(val);
        }
    }
    template<std::input_iterator LegacyInputIterators>
    circular_buffer(LegacyInputIterators i, LegacyInputIterators j)
    :   circular_buffer()
    {
        auto it = i;
        std::size_t count = j - i;
        capacity = count;
        m_size = count;
        allocator = Allocator();
        data = allocator.allocate(count);
        tail = count;
        for(int k = 0; k < count; ++k)
        {
            new (&data[k]) T(*it);
            ++it;
        }
    }

    circular_buffer(std::initializer_list<T> il)
    :   circular_buffer(il.begin(), il.end()) {} 

    circular_buffer(const Allocator& allo)
    :   circular_buffer()
    {
        allocator = allo;
    }
    circular_buffer(const circular_buffer& other, const Allocator& allo)
    :   circular_buffer(allo)
    {
        capacity = other.capacity;
        head = other.head;
        tail = other.tail;
        m_size = other.m_size;
        data = allocator.allocate(capacity);
        for(int i = 0; i < m_size; ++i)
        {
            std::size_t pos = (head + i) % capacity;
            new (&data[pos]) T(other.data[pos]);
        }
    }
    circular_buffer(circular_buffer&& other, const Allocator& allo = Allocator()) noexcept
    : allocator(allo), capacity(other.capacity), m_size(other.m_size), 
      head(other.head), tail(other.tail), data(other.data)
    {
        other.capacity = 0;
        other.m_size = 0;
        other.head = 0;
        other.tail = 0;
        other.data = nullptr;
    }
    void clear()
    {
        if (capacity == 0 || m_size == 0) return;
        for(std::size_t i = 0; i < m_size; ++i)
        {
            data[(i + head) % capacity]. ~T();
        }
        m_size = 0;
        tail = head = 0;
    }
    ~circular_buffer() 
    {
        clear();
        allocator.deallocate(data, capacity);
        data = nullptr;
    }
    circular_buffer(const circular_buffer& other)
    :   capacity(other.capacity),
        m_size(other.m_size),
        head(other.head),
        tail(other.tail),
        allocator(other.allocator)
    {
        data = allocator.allocate(capacity);
        for(std::size_t i = 0; i < m_size; ++i) 
        {
            std::size_t pos = (i + head) % capacity;
            new (&data[pos]) T(other.data[pos]);
        }
    }
    void swap(circular_buffer& other)
    {
        std::swap(capacity, other.capacity);
        std::swap(m_size, other.m_size);
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(allocator, other.allocator);
        std::swap(data, other.data);
    }
    circular_buffer& operator=(circular_buffer other)
    {
        this->swap(other);
        return *this;
    }
    circular_buffer& operator=(std::initializer_list<T> il)
    {   
        clear();
        allocator.deallocate(data, capacity);
        std::size_t count = il.end() - il.begin();
        capacity = count;
        m_size = count;
        head = 0;
        tail = count;
        data = allocator.allocate(count);
        const T* it = il.begin();
        for(std::size_t i = 0; i < m_size; ++i) 
        {
            new (&data[i]) T(*it);
            ++it;
        }
        return *this;
    }
    iterator begin()
    {
        return Iterator<T>(data, capacity, head, 0); 
    }
    const_iterator begin() const
    {
        return Iterator<const T>(data, capacity, head, 0); 
    }

    iterator end()
    {
        return Iterator<T>(data, capacity, head, m_size); 
    }
    const_iterator end() const
    {
        return Iterator<const T>(data, capacity, head, m_size); 
    }

    const_iterator cbegin() const 
    {
        return begin();
    }
    const_iterator cend() const
    {
        return end();
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    bool operator==(const circular_buffer& other) const
    {
        if(m_size != other.m_size) {return false;}
        for(int i = 0; i < m_size; ++i)
        {
            int dpos = (i + head) % capacity;
            int opos = (i + other.head) % other.capacity;
            if(data[dpos] != other.data[opos]) {return false;}
        }
        return true;
    }
    bool operator!=(const circular_buffer& other) const {return !(*this == other);}

    friend void swap(circular_buffer& lhs, circular_buffer& rhs) 
    {
        std::swap(lhs.capacity, rhs.capacity);
        std::swap(lhs.m_size, rhs.m_size);
        std::swap(lhs.head, rhs.head);
        std::swap(lhs.tail, rhs.tail);
        std::swap(lhs.allocator, rhs.allocator);
        std::swap(lhs.data, rhs.data);
    }

    size_type size() const
    {
        return m_size;
    }

    size_type max_size() const
    {
        return capacity;
    }

    bool empty() const
    {
        return (m_size == 0);
    }
    bool operator>(const circular_buffer& other) const 
    {
        for(int i = 0; i < std::min(m_size, other.m_size); ++i)
        {
            std::size_t dpos = (i + head) % capacity;
            std::size_t opos = (i + other.head) % other.capacity;
            if(data[dpos] > other.data[opos]) return true;  
            if(data[dpos] < other.data[opos]) {return false;}
        }
        return m_size > other.m_size;
    }
    bool operator<(const circular_buffer& other) const 
    {
        for(int i = 0; i < std::min(m_size, other.m_size); ++i)
        {
            std::size_t dpos = (i + head) % capacity;
            std::size_t opos = (i + other.head) % other.capacity;
            if(data[dpos] < other.data[opos]) return true;  
            if(data[dpos] > other.data[opos]) {return false;}
        }
        return m_size < other.m_size;
    }
    bool operator>=(const circular_buffer& other) const 
    {
        for(int i = 0; i < std::min(m_size, other.m_size); ++i)
        {
            std::size_t dpos = (i + head) % capacity;
            std::size_t opos = (i + other.head) % other.capacity;
            if(data[dpos] > other.data[opos]) return true;  
            if(data[dpos] < other.data[opos]) {return false;}
        }
        return m_size >= other.m_size;
    }
    bool operator<=(const circular_buffer& other) const 
    {
        for(int i = 0; i < std::min(m_size, other.m_size); ++i)
        {
            std::size_t dpos = (i + head) % capacity;
            std::size_t opos = (i + other.head) % other.capacity;
            if(data[dpos] < other.data[opos]) return true;  
            if(data[dpos] > other.data[opos]) {return false;}
        }
        return m_size <= other.m_size;
    }

    void CheckSize(std::size_t n)
    {   
        if(!IsExtendable) {
            return;
        }
        
        while(m_size + n > capacity)
        {
            Allocator new_allocator = Allocator();
            if(capacity == 0) {
                capacity = 1;
            }
            T* new_data = new_allocator.allocate(capacity * 2);
            for (int i = 0; i < m_size; ++i) {
                int pos = (i + head) % capacity;
                data[pos].~T();
                new (&new_data[i]) T(data[pos]);
            }
            allocator.deallocate(data, capacity);
            capacity *= 2;
            allocator = new_allocator;
            data = new_data;
            head = 0;
            tail = m_size;
        }
    }
    
    void resize(size_type count) {
        if(count < m_size) {
            for(int i = count; i < m_size; ++i) {
                data[(i + head) % capacity].~T();
            }
            m_size = count;
            tail = (head + m_size) % capacity;
        } else {
            if(count > m_size) {
                 CheckSize(count - m_size);
            }
            for(int i = m_size; i < count; ++i) {
                new (&data[(i + head) % capacity]) T();
            }
            m_size = count;
            tail = (head + m_size) % capacity;
        }
    }
    void resize(size_type count, const T& value) {
        if(count < m_size) {
            for(std::size_t i = count; i < m_size; ++i) {
                data[(i + head) % capacity].~T();
            }
            m_size = count;
            tail = (head + m_size) % capacity;
        } else {
            if(count > m_size) {
                CheckSize(count - m_size);
            }
            for(std::size_t i = m_size; i < count; ++i) {
                new (&data[(i + head) % capacity]) T(value);
            }
            m_size = count;
            tail = (head + m_size) % capacity;
        }
    }

    iterator insert(const_iterator it, const T& val)
    {
        std::size_t pos_it = it - begin();
        CheckSize(1);
        for(std::size_t i = m_size; i > pos_it; --i)
        {
            std::size_t from  = (head + i - 1 + capacity) % capacity;
            std::size_t to = (head + i + capacity) % capacity;
            new (&data[to]) T(data[from]);
            data[from].~T();
        }
        ++m_size;
        tail = (tail + 1) % capacity;
        new (&data[(pos_it + head) % capacity]) T(val);
        return begin() + pos_it;
    }
    iterator insert(iterator it, const T& val)
    {
        return insert(const_iterator(it), val);
    }
    iterator insert(const_iterator it, T&& val)
    {
        std::size_t pos_it = it - begin();
        CheckSize(1);
        for(std::size_t i = m_size; i > pos_it; --i)
        {
            std::size_t from  = (head + i - 1 + capacity) % capacity;
            std::size_t to = (head + i + capacity) % capacity;
            new (&data[to]) T(data[from]);
            data[from].~T();
        }
        ++m_size;
        tail = (tail + 1) % capacity;
        new (&data[(pos_it + head) % capacity]) T(std::move(val));
        return begin() + pos_it;
    }
    iterator insert(iterator it, T&& val)
    {
        return insert(const_iterator(it), std::move(val));
    }

    iterator insert(const_iterator it, size_type n, const T& val)
    {
        std::size_t pos_it = it - begin();
        CheckSize(n);
        for(std::size_t i = m_size; i > pos_it; --i)
        {
            std::size_t from  = (head + i - 1 + capacity) % capacity;
            std::size_t to = (head + i - 1 + n + capacity) % capacity;
            new (&data[to]) T(data[from]);
            data[from].~T();
        }
        for(std::size_t i = pos_it; i < pos_it + n; ++i)
        {
            new (&data[(head + i) % capacity]) T(val);
        }
        m_size += n;
        tail = (tail + n) % capacity;
        return begin() + pos_it;
    }
    iterator insert(iterator it, size_type n, const T& val)
    {
        return insert(const_iterator(it), n, val);
    }

    template<typename LegacyInputIterators>
    requires std::input_iterator<LegacyInputIterators>
    iterator insert(const_iterator it, LegacyInputIterators i, LegacyInputIterators j)
    {
        std::size_t count = std::distance(i, j);
        std::size_t pos_it = it - begin();
        CheckSize(count);
        for(std::size_t k = m_size; k > pos_it; --k)
        {
            std::size_t from = (k + head - 1 + capacity) % capacity;
            std::size_t to = (k + head - 1 + count + capacity) % capacity;
            new (&data[to]) T(data[from]);
            data[from].~T();
        }
        LegacyInputIterators it1 = i;
        for(std::size_t k = pos_it; k < pos_it + count; ++k)
        {
            new (&data[(head + k + capacity) % capacity]) T(*it1);
            ++it1;
        }
        m_size += count;
        tail = (tail + count) % capacity;
        return begin() + pos_it;
    }
    template<typename LegacyInputIterators>
    iterator insert(iterator it, LegacyInputIterators i, LegacyInputIterators j)
    {
        return insert(const_iterator(it), i, j);
    }
    iterator insert(const_iterator it, std::initializer_list<T> il)
    {
        return insert(it, il.begin(), il.end());
    }
    iterator insert(iterator it, std::initializer_list<T> il)
    {
        return insert(const_iterator(it), il.begin(), il.end());
    }
    iterator erase(const_iterator it)
    {
        std::size_t pos = it - begin();
        data[(head + pos) % capacity].~T();
        for (std::size_t i = pos; i < m_size - 1; i++)
        {
            new (&data[(head + i) % capacity]) T(data[(head + i + 1) % capacity]);
            data[(head + i + 1) % capacity].~T();
        }
        m_size -= 1;
        tail = (tail - 1 + capacity) % capacity;
        return begin() + pos;
    }
  
    iterator erase(const_iterator a, const_iterator b)
    {
        std::size_t count = b - a;
        std::size_t pos = a - begin();

        for(std::size_t i = pos; i < pos + count; ++i) { 
            data[(head + i) % capacity].~T();
        }
        for(std::size_t i = pos; i < m_size - count; ++i)
        {
            new (&data[(head + i) % capacity]) T(data[(head + i + count) % capacity]);
            data[(head + i + count) % capacity].~T();
        }
        m_size -= count;
        tail = (head + m_size) % capacity; 
        return begin() + pos;
    }
    template<typename LegacyInputIterators>
    requires std::input_iterator<LegacyInputIterators>
    void assign(LegacyInputIterators i, LegacyInputIterators j)
    {
        clear();
        LegacyInputIterators it = i;
        std::size_t count = j - i;
        CheckSize(count);
        for(std::size_t k = 0; k < count; ++k)
        {

            new (&data[(head + k) % capacity]) T(*it);
            ++it;
        }
        m_size = count;
        tail = (head + count) % capacity;
    }
    void assign(std::initializer_list<T> il)
    {
        assign(il.begin(), il.end());
    }
    void assign(size_type n, const T& t)
    {
        clear();
        CheckSize(n);
        for(std::size_t i = 0; i < n; ++i)
        {
            new (&data[i]) T(t);
        }
        tail = n;
        m_size = n;
    }
    reference front() 
    {
        return *begin();
    }
    const_reference front() const
    {
        return *begin();
    }
    reference back()
    {
        return *(end() - 1);
    }
    const_reference back() const
    {
        return *(end() - 1);
    }
    void push_front(const T& val)
    {
        CheckSize(1);
        if (m_size == capacity && capacity > 0) { 
            head = (head - 1 + capacity) % capacity;
            data[head].~T(); 
            new (&data[head]) T(val);
            tail = (tail - 1 + capacity) % capacity; 
        } else {
            head = (head - 1 + capacity) % capacity;
            new (&data[head]) T(val);
            m_size++;
        }
    }
    void push_front(T&& val)
    {
        CheckSize(1);
        if (m_size == capacity && capacity > 0) { 
            head = (head - 1 + capacity) % capacity;
            data[head].~T(); 
            new (&data[head]) T(val);
            tail = (tail - 1 + capacity) % capacity; 
        } else {
            head = (head - 1 + capacity) % capacity;
            new (&data[head]) T(std::move(val));
            m_size++;
        }
    }
    void push_back(const T& val) 
    {
        CheckSize(1);
        if (m_size == capacity && capacity > 0) {
            data[tail].~T(); 
            new (&data[tail]) T(val);
            tail = (tail + 1) % capacity;
            head = (head + 1) % capacity;
        } else {
            new (&data[tail]) T(val);
            tail = (tail + 1) % capacity;
            m_size += 1;
        }
    }
    void push_back( T&& val) 
    {
        CheckSize(1);
        if (m_size == capacity && capacity > 0) {
            data[tail].~T(); 
            new (&data[tail]) T(std::move(val));
            tail = (tail + 1) % capacity;
            head = (head + 1) % capacity;
        } else {
            new (&data[tail]) T(std::move(val));
            tail = (tail + 1) % capacity;
            m_size += 1;
        }
    }
    void pop_front()
    {
        if (empty()) throw std::out_of_range("empty buffer");
        m_size -= 1;
        data[head].~T();
        head = (head + 1) % capacity;
    }
    void pop_back()
    {
        if (empty()) throw std::out_of_range("empty buffer");
        m_size -= 1;
        data[(tail - 1 + capacity) % capacity].~T();
        tail = (tail - 1 + capacity) % capacity;
    }
    reference operator[](size_type n)
    {
        return data[(n + head) % capacity];
    }
    const_reference operator[](size_type n) const
    {
        return data[(n + head) % capacity];
    }
    reference at(size_type n)
    {
        return data[(n + head) % capacity];
    }
    const_reference at(size_type n) const
    {
        return data[(n + head) % capacity];
    }
    reverse_iterator rbegin()
    {
        return ReverseIterator<T>(end());
    }
    reverse_iterator rend()
    {
        return ReverseIterator<T>(begin());
    }
    const_reverse_iterator rbegin() const 
    {
        return ReverseIterator<const T>(end());
    }
    const_reverse_iterator rend() const 
    {
        return ReverseIterator<const T>(begin());
    }
    allocator_type get_allocator() const
    {
        return allocator;
    }
};