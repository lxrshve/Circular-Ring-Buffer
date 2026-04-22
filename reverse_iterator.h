#include <iterator> 
#include <cstddef> 
#include "iterator.h"
template <typename T>
class ReverseIterator 
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T&; 
    using pointer = T*; 
    
    ReverseIterator() 
    :    it() {}            
    ReverseIterator(Iterator<T> it2) : it(it2) {}
    ReverseIterator& operator=(const ReverseIterator& other) 
    {
        if(this != &other)
        {
            it = other.it;
        }
        return *this;
    }
    reference operator*()
    {
        Iterator<T> tmp = it;
        --tmp;
        return *tmp;
    }
    const T& operator*() const {
         Iterator<T> tmp = it;
        --tmp;
        return *tmp;
    }
    T* operator->()
    {
        Iterator<T> tmp = it;
        --tmp;
        return tmp.operator->();
    }
    const T* operator->() const
    {
        Iterator<T> tmp = it;
        --tmp;
        return tmp.operator->();
    }
    reference operator[](difference_type n) const
    {
        return *(it - n - 1);
    }
    ReverseIterator& operator++()
    {
        it--;
        return * this;
    }
    ReverseIterator& operator+=(difference_type n)
    {
        it.operator-=(n);
        return *this;
    }
    ReverseIterator operator+(difference_type n) const
    {
        return ReverseIterator(it.operator-(n));
    }
    ReverseIterator operator++(int)
    {
        ReverseIterator old = *this;
        it--;
        return old;
    }
    ReverseIterator& operator--()
{
    it++;
    return *this;
}
    ReverseIterator operator--(int)
    {
        ReverseIterator old = *this;
        it++;
        return old;
    }
    ReverseIterator operator-(difference_type n) const
    {
        return ReverseIterator(it + n);
    }

    template <typename Iter1, typename Iter2>
    friend bool operator==(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b);
    template <typename Iter1, typename Iter2>
    friend bool operator!=(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b);
    template <typename Iter1, typename Iter2>
    friend bool operator<(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b);
    template <typename Iter1, typename Iter2>
    friend bool operator>(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b);

    template <typename Iter1, typename Iter2>
    friend bool operator<=(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b);

    template <typename Iter1, typename Iter2>
    friend bool operator>=(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b);

    template <typename Iter>
    friend ReverseIterator<Iter> operator+(std::ptrdiff_t n, const ReverseIterator<Iter>& it);

    template <typename Iter1, typename Iter2>
    friend std::ptrdiff_t operator-(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b);

    template <typename Iter>
    friend ReverseIterator<Iter> make_reverse_iterator(Iterator<Iter> it);

    template <typename Iter1, typename Iter2>
    friend void iter_swap(ReverseIterator<Iter1> a, ReverseIterator<Iter2> b);

    
private:
    Iterator<T> it;
};
template <typename Iter1, typename Iter2>
    bool operator==(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b)
    {
        return a.it == b.it;
    }

    template <typename Iter1, typename Iter2>
    bool operator!=(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b)
    {
        return a.it != b.it;
    }

    template <typename Iter1, typename Iter2>
    bool operator<(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b)
    {
        return a.it > b.it;
    }

    template <typename Iter1, typename Iter2>
    bool operator>(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b)
    {
        return a.it < b.it;
    }

    template <typename Iter1, typename Iter2>
    bool operator<=(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b)
    {
        return a.it >= b.it;
    }

    template <typename Iter1, typename Iter2>
    bool operator>=(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b)
    {
        return a.it <= b.it;
    }

    template <typename Iter>
    ReverseIterator<Iter> operator+(std::ptrdiff_t n, const ReverseIterator<Iter>& it)
    {
        return ReverseIterator<Iter>(it.it - n);
    }

    template <typename Iter1, typename Iter2>
    std::ptrdiff_t operator-(const ReverseIterator<Iter1>& a, const ReverseIterator<Iter2>& b)
    {
        return b.it - a.it;
    }

    template <typename Iter>
    ReverseIterator<Iter> make_reverse_iterator(Iterator<Iter> it)
    {
        return ReverseIterator<Iter>(it);
    }

    template <typename Iter1, typename Iter2>
    void iter_swap(ReverseIterator<Iter1> a, ReverseIterator<Iter2> b)
    {
        std::swap(*a, *b);
    }