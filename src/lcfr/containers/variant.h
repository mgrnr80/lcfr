#pragma once

#include <stdint.h>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>

namespace lcfr
{

namespace variant_detail
{

template <typename X, typename... Ts>
struct finder;

template <typename X, typename T, typename... Ts>
struct finder<X, T, Ts...> {
    static const int index = (finder<X, Ts...>::index == -1) ? -1 : 1 + finder<X, Ts...>::index;
};

template <typename X, typename... Ts>
struct finder<X, X, Ts...> {
    static const int index = 0;
};

template <typename X>
struct finder<X> {
    static const int index = -1;
};

template <typename... Ts>
struct max_size_of;

template <typename X, typename... Ts>
struct max_size_of<X, Ts...> {
    static const size_t size = (sizeof(X) > max_size_of<Ts...>::size) ? sizeof(X) : max_size_of<Ts...>::size;
};

template <typename X>
struct max_size_of<X> {
    static const size_t size = sizeof(X);
};

template<int N, typename... Ts>
struct deleter;

template<int N>
struct deleter<N> {
    static void execute(int n, const void* ptr) {}
};

template<int N, typename T, typename... Ts>
struct deleter<N, T, Ts...> {
    static void execute(int n, const void* ptr)
    {
        if (n == N) reinterpret_cast<const T*>(ptr)->~T();
        else deleter<N + 1, Ts...>::execute(n, ptr);
    }
};

template <int N, typename... Ts>
struct copy_constructor;

template<int N>
struct copy_constructor<N> {
    static void execute(int n, char* obj, const char* other) {}
};

template<int N, typename T, typename... Ts>
struct copy_constructor<N, T, Ts...> {
    static void execute(int n, char* obj, const char* other)
    {
        if (n == N) new(obj) T(*reinterpret_cast<const T*>(other));
        else copy_constructor<N + 1, Ts...>::execute(n, obj, other);
    }
};

template <int N, typename... Ts>
struct move_constructor;

template<int N>
struct move_constructor<N> {
    static void execute(int n, char* obj, char* other) {}
};

template<int N, typename T, typename... Ts>
struct move_constructor<N, T, Ts...> {
    static void execute(int n, char* obj, char* other)
    {
        if (n == N) std::swap(*reinterpret_cast<T*>(other), *(new(obj) T()));
        else move_constructor<N + 1, Ts...>::execute(n, obj, other);
    }
};

template<typename B, typename... Ts>
struct caster;

template<typename B>
struct caster<B> {
    static const B* execute(int n, const void* ptr) { throw std::bad_cast(); }
};

template<typename B, typename T, typename... Ts>
struct caster<B, T, Ts...> {
    static const B* execute(int n, const void* ptr)
    {
        if (n == 0)
        {
            if (std::is_base_of<B, T>::value) return reinterpret_cast<const B*>(ptr);
            else throw std::bad_cast();
        }
        else return caster<B, Ts...>::execute(n - 1, ptr);
    }
};


template <int N, typename... Ts>
struct type_at;

template <typename T, typename... Ts>
struct type_at<0, T, Ts...> {
    typedef T result;
};

template <int N, typename T, typename... Ts>
struct type_at<N, T, Ts...> {
    typedef typename type_at<N - 1, Ts...>::result result;
};

}

template <typename... Ts>
class variant
{
public:
    variant()
        : which_(-1)
    {}

    variant(const variant& obj)
        : which_(obj.which_)
    {
        variant_detail::copy_constructor<0, Ts...>::execute(which_, u_.data_, obj.u_.data_);
    }

    variant(variant&& obj)
        : which_(obj.which_)
    {
        variant_detail::move_constructor<0, Ts...>::execute(which_, u_.data_, obj.u_.data_);
    }

    template <typename T>
    variant(const T& obj)
    {
        static_assert(variant_detail::finder<T, Ts...>::index != -1, "invalid type");
        which_ = variant_detail::finder<T, Ts...>::index;
        new(u_.data_) T(obj);
    }

    ~variant()
    {
        destroy();
    }

    template <class T> const variant& operator=(const T& obj)
    {
        static_assert(variant_detail::finder<T, Ts...>::index != -1, "invalid type");
        if (is<T>())
        {
            as<T>() = obj;
        }
        else
        {
            if (!isEmpty()) destroy();
            which_ = variant_detail::finder<T, Ts...>::index;
            new(u_.data_) T(obj);
        }
        return *this;
    }

    template <class T, typename... Args> void emplace(Args&&... args)
    {
        static_assert(variant_detail::finder<T, Ts...>::index != -1, "invalid type");
        if (!isEmpty()) destroy();
        which_ = variant_detail::finder<T, Ts...>::index;
        new(u_.data_) T(std::forward<Args>(args)...);
    }

    template <class T> const T& as() const
    {
        //checkType<T>();
        //return *reinterpret_cast<const T*>(u_.data_);
        if (is<T>()) return *reinterpret_cast<const T*>(u_.data_);
        else return *variant_detail::caster<T, Ts...>::execute(which_, u_.data_);
    }

    template <class T> T& as()
    {
        if (is<T>()) return *reinterpret_cast<T*>(u_.data_);
        else return *const_cast<T*>(variant_detail::caster<T, Ts...>::execute(which_, u_.data_));
    }

    template <class T> bool is() const
    {
        return (which_ >= 0) && (which_ == variant_detail::finder<T, Ts...>::index);
    }

    bool isEmpty() const
    {
        return (which_ == -1);
    }

    const variant& operator=(const variant& obj)
    {
        destroy();
        new(this) variant(obj);
        return *this;
    }

private:

    template <class T> void checkTypeStrict() const
    {
        if (which_ != variant_detail::finder<T, Ts...>::index)
            throw std::bad_cast();
    }

    void destroy() const
    {
        variant_detail::deleter<0, Ts...>::execute(which_, u_.data_);
    }

    int which_;

    union {
        char    data_[variant_detail::max_size_of<Ts...>::size];
        int32_t dummy_int32_;
        int64_t dummy_int64_;
        double  dummy_double_;
    } u_;
};

}
