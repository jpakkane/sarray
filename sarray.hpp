/**
Copyright 2019 Jussi Pakkanen

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#pragma once

#include<optional>
#include<functional>
#include<utility>

namespace s {

#if 1
template<class T> class maybe final {
public:

    maybe(): item(nullptr), backing{} {}
    explicit maybe(T* item) : item(item), backing{} {}

    maybe(maybe&&o) {
        backing = o.item ? std::move(*o.item) : std::move(backing);
        item = nullptr;
    }

    T& operator*() {
        if(!item) {
           return backing;
        }
        return *item;
    }

    void swap(maybe<T> &other) {
        if(!item) { // Not really correct but worksfornow.
            std::abort();
        }
        if(!other.item) {
            std::abort();
        }
        T tmp = *item;
        *item = *other.item;
        *other.item = tmp;
        tmp = backing;
        backing = other.backing;
        other.backing = backing;
    }

    const T& operator*() const {
        if(!item) {
            std::abort();
        }
        return *item;
    }
/*
    maybe& operator=(const maybe &other) {
        if(!item) {
            item = other.item;
        } else if (!other.item) {
            item = nullptr;
        } else {
            *item = *other.item;
        }
        return *this;
    }
*/
    maybe& operator=(maybe &&other) {
        if(!item) {
            if(!other.item) {
                backing = std::move(other.backing);
            } else {
                backing = std::move(*other.item);
            }
        } else {
            if (!other.item) {
                *item = std::move(other.backing);
            } else {
                *item = std::move(*other.item);
            }
        }
        return *this;
    }

    bool operator<(const maybe &other) const {
        if(!item) {
            if(!other.item) {
                return backing < other.backing;
            } else {
                return backing < *other.item;
            }
        } else {
            if (!other.item) {
                return *item < other.backing;
            } else {
                return *item < *other.item;
            }
        }
    }

    explicit operator bool() const {
        return item;
    }

    T& value_or(T& default_value) const {
        return item ? *item : default_value;
    }


private:
    T* item;
    T backing;
};
template<typename T>
void swap(maybe<T> &&a, maybe<T> &&b) {
    a.swap(b);
}
#else

template <typename T> using maybe = std::optional<std::reference_wrapper<T>>;

template<typename T>
void swap(maybe<T> &&a, maybe<T> &&b) {
    a.get().swap(b.get());
}
#endif

template<class T, int size>
class array final {

public:

    typedef T value_type;

    template<class D> class array_unsafe {
    public:
        array_unsafe(D *main) : main(main) {}

        auto begin() { return main->data; }
        auto end() { return main->data + size; }

    private:
        array<T, size> *main;
    };


    template<class D> class iterator final {
    public:

        typedef maybe<typename D::value_type> value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef size_t difference_type;
        typedef ::std::random_access_iterator_tag iterator_category;

        iterator(D *main, int index) : main(main), index(index) {}

        iterator(const iterator &other) : main(other.main), index(other.index) {}

        value_type operator*() {
            return (*main)[index];
        }

        explicit operator bool() const {
            return main->is_valid_index(index);
        }

        T& value_or(T& default_value) const {
            return main->value_or(index, default_value);
        }

        bool operator==(const iterator<D> &other) const {
            //assert(main == other.main);
            return index == other.index;
        }

        bool operator!=(const iterator<D> &other) const {
            return !(*this == other);
        }

        int operator-(const iterator<D> &other) const {
            return index - other.index;
        }

        int operator<(const iterator<D> &other) const {
            return index < other.index;
        }

        iterator<D> operator+(int i) const {
            return iterator<D>(main, index + i);
        }


        iterator<D> operator-(int i) const {
            return iterator<D>(main, index - i);
        }

        iterator<D>& operator++() {
            ++index;
            return *this;
        }

        iterator<D>& operator--() {
            --index;
            return *this;
        }

        iterator& operator=(const iterator &other) {
            main = other.main;
            index = other.index;
            return *this;
        }

    private:
        array<T, size> *main;
        int index;

    };

    array() : data{} {}

    maybe<T> operator[](const int index) {
        if(is_valid_index(index)) {
            return maybe<T>{&(data[index])};
        } else {
            return maybe<T>{};
        }
    }

    array::iterator<array<T, size>> begin() {
        return array::iterator<array<T, size>>(this, 0);
    }

    array::iterator<array<T, size>> end() {
        return array::iterator<array<T, size>>(this, size);
    }

    bool is_valid_index(const int index) const {
        if(index < 0 || index >= size) {
            return false;
        }
        return true;
    }

    T& value_or(const int index, T& default_value) {
        if(is_valid_index(index)) {
            return data[index];
        }
        return default_value;
    }

    array::array_unsafe<array<T, size>> unsafe() { return array_unsafe(this); }

private:

    T& unsafe_index(const int index) {
        return data[index];
    }
    T data[size];
};

}
