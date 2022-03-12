#include <ios>
#include <stdint.h>
#include <iostream>
#include <type_traits>
#include <vector>
#include <list>

#include "my_type_traits.hpp"


template <typename T>
std::enable_if_t<std::is_integral<T>::value, void> print_ip(T v) {
    bool start_print = false;

    for (int i = sizeof(T) - 1; i >= 0; --i) {
        int val = static_cast<uint8_t>(v >> (i * 8));
        if (start_print) {
            std::cout << ".";
        }
        if (val || !i) {
            start_print = true;
        }
        if (start_print) {
            std::cout << val;
        }
    }

    std::cout << std::endl;
}

template <typename T>
std::enable_if_t<is_container<T>::value, void> print_ip(T v) {
    bool first = true;

    for (const auto& a : v) {
        if (!first) {
            std::cout << ".";
        }
        std::cout << a;
        first = false;
    }
    std::cout << std::endl;
}

template <typename T>
std::enable_if_t<is_string<T>::value, void> print_ip(T v) {
    std::cout  << v << std::endl;
}

int main(int, char *[]) {

    print_ip(char(-1));
    print_ip(short(0));
    print_ip(int(2130706433));
    print_ip(long(8875824491850138409));
    print_ip(std::string("12.3.18.10"));
    print_ip(std::vector<int>{15, 13, 10, 255});
    print_ip(std::list<short>{123, 124, 30, 40});

    return 0;
}

