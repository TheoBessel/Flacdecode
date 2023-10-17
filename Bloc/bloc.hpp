#ifndef BLOC_HPP
#define BLOC_HPP

#include "../BitInput/bitinput.hpp"
#include <iostream>

enum Order {BIG_ENDIAN_ORDER, LITTLE_ENDIAN_ORDER};

class Bloc {
public:
    Bloc(std::shared_ptr<BitInput> inp);
    bool is_valid();
    bool is_last();
    void read_header();
    uint64_t read_size(size_t n, bool reverse = false);
    void read_body();
    void print_info();
protected: // Only accessible by children
    std::shared_ptr<BitInput> m_inp;
    bool m_last;
    uint64_t m_type;
    uint64_t m_length;
};

#endif // BLOC_HPP