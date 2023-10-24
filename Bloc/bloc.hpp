#ifndef BLOC_HPP
#define BLOC_HPP

#include "../BitInput/bitinput.hpp"
#include <iostream>

enum class Order {BIG_ENDIAN_ORDER, LITTLE_ENDIAN_ORDER};

class Bloc {
public:
    Bloc() = default;
    Bloc(std::shared_ptr<BitInput> inp);
    void read_header();
    void read_body();
    void print_info();
    uint64_t read_size(const uint64_t& n, Order reverse = Order::BIG_ENDIAN_ORDER);
    std::string read_string(const uint64_t& length);
    uint64_t silent_read_type();
    void update_bloc(const bool& last, const uint64_t& type, const uint64_t& length);

    std::shared_ptr<BitInput> get_inp() { return m_inp; }
    bool is_last() { return m_last; }
    uint64_t get_type() { return m_type; }
    u_int64_t get_length() { return m_length; }

protected: // Only accessible by children
    std::shared_ptr<BitInput> m_inp;
    bool m_last;
    uint64_t m_type;
    uint64_t m_length;
};

#endif // BLOC_HPP