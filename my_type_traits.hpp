#pragma once
#include <stdint.h>
#include <vector>
#include <list>

template <typename T>
struct is_string;

#define USE_MY_OWN_VERSION 1

#if USE_MY_OWN_VERSION
// this implementation is my own, and not so beautiful

template <typename T> struct is_vector : std::false_type {};
template <typename T> struct is_vector<std::vector<T>> : std::true_type {};

template <typename T> struct is_list : std::false_type {};
template <typename T> struct is_list<std::list<T>> : std::true_type {};

template <typename T>
struct is_container : std::integral_constant<bool, is_list<T>::value || is_vector<T>::value > {};

#else
// this implementation is spizheno from the internet, and works great

template<typename T>
struct has_const_iterator
{
private:
    typedef char                      yes;
    typedef struct { char array[2]; } no;

    template<typename C> static yes test(typename C::const_iterator*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template <typename T>
struct has_begin_end
{
    template<typename C>
    static char (&f(
                typename std::enable_if<
                      std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
                                   typename C::const_iterator(C::*)() const>::value,
                               void>::type*
                   ))[1];

    template<typename C> static char (&f(...))[2];

    template<typename C> static char (&g(typename std::enable_if<
      std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
      typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&g(...))[2];

    static bool const beg_value = sizeof(f<T>(0)) == 1;
    static bool const end_value = sizeof(g<T>(0)) == 1;
};

template<typename T> 
struct is_container : std::integral_constant<bool, !is_string<T>::value && has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value> { };
#endif  // USE_MY_OWN_VERSION

template <typename T>
struct is_string {

    template <typename U>
    static uint8_t test(typename 
            std::enable_if<
                std::is_same<typename U::value_type, char>::value ||
                std::is_same<typename U::value_type, wchar_t>::value
                , void>::type*);

    template <typename U>
    static uint16_t test(...);

    static const bool value = sizeof(test<T>(0)) == sizeof(uint8_t);
};

template<typename T>
inline constexpr bool is_string_v = is_string<T>::value;

