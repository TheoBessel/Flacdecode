#include "frame.hpp"

Frame::Frame(std::shared_ptr<BitInput> inp) : 
    m_inp (inp), 
    m_sync_code (0), 
    m_blocking_strategy (0), 
    m_channels (0),
    m_bitrate (0),
    m_crc8 (0),
    m_crc16 (0) {}

void Frame::read_header() {
    m_sync_code = m_inp->read_uint(14);
    m_inp->read_uint(1);
    m_blocking_strategy = m_inp->read_uint(1);
    m_blocksize = m_inp->read_uint(4);
    m_samplerate = m_inp->read_uint(4);
    m_channels = m_inp->read_uint(4);
    m_bitrate = m_inp->read_uint(3);
    m_inp->read_uint(1);
    /*if (m_blocking_strategy) {
        m_inp->read_uint(36);
    } else {
        m_inp->read_uint(31);
    }*/
}

void Frame::print_info() {
    std::cout << std::endl;
    std::cout << "---- [Frame infos] ----" << std::endl;
    std::cout << "[https://www.ietf.org/archive/id/draft-ietf-cellar-flac-12.txt]" << std::endl;
    std::cout << "   - Sync    : " << std::bitset<8>(m_sync_code) << std::endl;
    std::cout << "   - Strat   : " << std::bitset<1>(m_blocking_strategy) << std::endl;
    std::cout << "   - Size    : " << std::bitset<4>(m_blocksize) << std::endl;
    std::cout << "   - Sample  : " << std::bitset<4>(m_samplerate) << std::endl;
    std::cout << "   - Channel : " << std::bitset<4>(m_channels) << std::endl;
    std::cout << "   - Bitrate : " << std::bitset<3>(m_bitrate) << std::endl;
    std::cout << "-------- [End] --------" << std::endl << std::endl;
}
