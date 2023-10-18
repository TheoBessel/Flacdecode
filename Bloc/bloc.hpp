#ifndef BLOC_HPP
#define BLOC_HPP

#include "../BitInput/bitinput.hpp"
#include <iostream>

enum class Order {BIG_ENDIAN_ORDER, LITTLE_ENDIAN_ORDER};

class Bloc {
public:
    Bloc(std::shared_ptr<BitInput> inp);
    bool is_last();
    void read_header();
    void read_body();
    void print_info();
    uint64_t read_size(size_t n, Order reverse = Order::BIG_ENDIAN_ORDER);
    std::string read_string(uint64_t length);
protected: // Only accessible by children
    std::shared_ptr<BitInput> m_inp;
    bool m_last;
    uint64_t m_type;
    uint64_t m_length;
};

#endif // BLOC_HPP