#include "bloc.hpp"
#include <iostream>

Bloc::Bloc(std::shared_ptr<BitInput> inp) : m_inp (inp), m_last (false), m_type (0), m_length (0) {}

bool Bloc::is_valid() {
    uint64_t header = m_inp->read_uint(32);
    bool valid = (header == static_cast<uint64_t>(1716281667));
    return valid;
}

bool Bloc::is_last() {
    return m_last;
}

void Bloc::read_header() {
    uint64_t last = m_inp->read_uint(1);
    m_last = last != 0;
    m_type = m_inp->read_uint(7);
    m_length = m_inp->read_uint(24);
}

uint64_t Bloc::read_size(size_t n, bool reverse) {
    uint64_t size = m_inp->read_uint(n);
    if (reverse) {
        return __builtin_bswap32(size);
    } else {
        return size;
    }
}

void Bloc::read_body() {
        m_inp->read_uint(8*m_length);
}

void Bloc::print_info() {
    std::string i_type = "";
    std::string i_last = "";
    if (m_last) {i_last = "yes";} else {i_last = "no";}
    switch (m_type) {
        case 0: i_type = "STREAMINFO"; break;
        case 1: i_type = "PADDING"; break;
        case 2: i_type = "APPLICATION"; break;
        case 3: i_type = "SEEKTABLE"; break;
        case 4: i_type = "VORBIS_COMMENT"; break;
        case 5: i_type = "CUESHEET"; break;
        case 6: i_type = "PICTURE"; break;
        case 127: i_type = "invalid, to avoid confusion with a frame sync code"; break;
        default: i_type = "reserved";
    }
    std::cout << std::endl;
    std::cout << "---- [Bloc infos] ----" << std::endl;
    std::cout << "   - Last   : " << i_last << std::endl;
    std::cout << "   - Type   : " << i_type << std::endl;
    std::cout << "   - Length : " << m_length << std::endl;
    std::cout << "-------- [End] --------" << std::endl << std::endl;
}