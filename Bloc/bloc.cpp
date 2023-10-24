#include "bloc.hpp"

Bloc::Bloc(std::shared_ptr<BitInput> inp) : 
    m_inp (inp), 
    m_last (false), 
    m_type (0), 
    m_length (0) {}

void Bloc::read_header() {
    uint64_t last = m_inp->read_uint(1);
    m_last = last != 0;
    m_type = m_inp->read_uint(7);
    m_length = m_inp->read_uint(24);
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
    std::cout << "----- [Bloc info] -----" << std::endl;
    std::cout << "   - Last    : " << i_last << std::endl;
    std::cout << "   - Type    : " << i_type << std::endl;
    std::cout << "   - Length  : " << m_length << std::endl;
    std::cout << "-------- [End] --------" << std::endl << std::endl;
}

uint64_t Bloc::read_size(const uint64_t& n, Order reverse) {
    uint64_t size = m_inp->read_uint(n);
    if (reverse == Order::LITTLE_ENDIAN_ORDER) {
        return __builtin_bswap32(size);
    } else {
        return size;
    }
}

std::string Bloc::read_string(const uint64_t& length) {
    std::string buffer = "";
    char charbuffer = ' ';
	for (uint64_t i = 0; i < length; ++i) {
		// Read UTF-8 characters
		charbuffer = static_cast<char>(this->m_inp->read_uint(8));
		buffer.push_back(charbuffer);
	}
    return buffer;
}

uint64_t Bloc::silent_read_type() {
    uint64_t mask = (1 << 7) - 1;
    uint64_t buffer = m_inp->read_uint(8, false);
    uint64_t type = buffer & mask;
    return type;
}

void Bloc::update_bloc(const bool& last, const uint64_t& type, const uint64_t& length) {
    this->m_last = last;
    this->m_type = type;
    this->m_length = length;
}