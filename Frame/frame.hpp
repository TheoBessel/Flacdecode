#ifndef FRAME_HPP
#define FRAME_HPP

#include "../BitInput/bitinput.hpp"
#include <iostream>

class Frame {
public:
    Frame(std::shared_ptr<BitInput> inp);
    void read_header();
    void read_subframes();
    void read_footer();
    void print_info();
    
protected: // Only accessible by children
    std::shared_ptr<BitInput> m_inp;
    uint64_t m_sync_code;
    uint64_t m_blocking_strategy;
    uint64_t m_blocksize;
    uint64_t m_samplerate;
    uint64_t m_channels;
    uint64_t m_bitrate;
    uint64_t m_crc8;
    uint64_t m_crc16;
};

#endif // FRAME_HPP