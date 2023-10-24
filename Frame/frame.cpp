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

    if (m_blocking_strategy) {
        m_inp->read_uint(56); // TODO : Cast the 56 bits int into a 36 bits utf-8 int
    } else {   
        m_inp->read_uint(48); // TODO : Cast the 48 bits int into a 31 bits utf-8 int
    }

    if (m_blocksize & 0b1110 == 0110) {
        m_blocksize = m_inp->read_uint(8) + 1;
    }

    if (m_samplerate & 0b1100 == 1100) {
        m_samplerate = m_inp->read_uint(8);
    }

    m_crc8 = m_inp->read_uint(8);

    std::tuple<std::vector<int64_t>, std::vector<int64_t> > subfr = this->read_subframes(); // m_blocksize, m_bitrate, m_channels

    std::cout << "Subframe 1 : ";
    for (size_t i = 0; i < std::get<0>(subfr).size(); i++) {
        std::cout << std::get<0>(subfr)[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Subframe 2 : ";
    for (size_t i = 0; i < std::get<1>(subfr).size(); i++) {
        std::cout << std::get<1>(subfr)[i] << " ";
    }
    std::cout << std::endl;

    this->read_footer();
}

std::tuple<std::vector<int64_t>, std::vector<int64_t> > Frame::read_subframes() {
    // Suppose that m_channels == 0b1000
    std::vector<int64_t> left = read_subframe(); // /!\ signed
    std::vector<int64_t> right = read_subframe(); // /!\ signed

    return {left, right};
}

// TODO : Create a class for subframes
std::vector<int64_t> Frame::read_subframe() { // /!\ signed
    m_inp->read_uint(1);
    uint64_t type = m_inp->read_uint(6);
    uint64_t shift = m_inp->read_uint(1);
    if (shift) {
        while (!m_inp->read_uint(1)) {
            shift++;
        }
    }
    m_bitrate -= shift;

    std::cout << "[TYPE] : " << type << std::endl;
    // TODO : Handle different types

    return std::vector<int64_t>(10); // Stupid array [0 0 0 0 0 0 0 0 0 0] returned, to change
}

void Frame::read_footer() {
    m_crc16 = m_inp->read_uint(16);
}

void Frame::print_info() {
    std::string i_blocking_strategy = "";
    std::string i_blocksize = "";
    std::string i_samplerate = "";
    std::string i_channels = "";
    std::string i_bitrate = "";
    switch (m_blocking_strategy) {
        case 0: i_blocking_strategy = "fixed-blocksize"; break;
        case 1: i_blocking_strategy = "variable-blocksize"; break;
    }
    switch (m_blocksize) {
        case 0b0000: i_blocksize = "reserved"; break;
        case 0b0001: i_blocksize = "192 samples"; break;
        case 0b0010 ... 0b0101: i_blocksize = "576 * (2^(n-2)) samples"; break;
        case 0b0110: i_blocksize = "get 8 bit (blocksize-1) from end of header"; break;
        case 0b0111: i_blocksize = "get 16 bit (blocksize-1) from end of header"; break;
        case 0b1000 ... 0b1111: i_blocksize = "256 * (2^(n-8)) samples"; break;
    }
    switch (m_samplerate) {
        case 0b0000: i_samplerate = "get from STREAMINFO metadata block"; break;
        case 0b0001: i_samplerate = "88.2kHz"; break;
        case 0b0010: i_samplerate = "176.4kHz"; break;
        case 0b0011: i_samplerate = "192kHz"; break;
        case 0b0100: i_samplerate = "8kHz"; break;
        case 0b0101: i_samplerate = "16kHz"; break;
        case 0b0110: i_samplerate = "22.05kHz"; break;
        case 0b0111: i_samplerate = "24kHz"; break;
        case 0b1000: i_samplerate = "32kHz"; break;
        case 0b1001: i_samplerate = "44.1kHz"; break;
        case 0b1010: i_samplerate = "48kHz"; break;
        case 0b1011: i_samplerate = "96kHz"; break;
        case 0b1100: i_samplerate = "get 8 bit sample rate (in kHz) from end of header"; break;
        case 0b1101: i_samplerate = "get 16 bit sample rate (in Hz) from end of header"; break;
        case 0b1110: i_samplerate = "get 16 bit sample rate (in tens of Hz) from end of header"; break;
        case 0b1111: i_samplerate = "invalid, to prevent sync-fooling string of 1s"; break;
    }
    switch (m_channels) {
        case 0b0000 ... 0b0111: i_channels = std::to_string(m_channels + 1); break;
        case 0b1000: i_channels = "left/side stereo"; break;
        case 0b1001: i_channels = "right/side stereo"; break;
        case 0b1010: i_channels = "mid/side stereo"; break;
        case 0b1011 ... 0b1111: i_channels = "reserved"; break;
    }
    switch (m_bitrate) {
        case 0b000: i_bitrate = "get from STREAMINFO metadata block"; break;
        case 0b001: i_bitrate = "8 bits per sample"; break;
        case 0b010: i_bitrate = "12 bits per sample"; break;
        case 0b011: i_bitrate = "reserved"; break;
        case 0b100: i_bitrate = "16 bits per sample"; break;
        case 0b101: i_bitrate = "20 bits per sample"; break;
        case 0b110: i_bitrate = "24 bits per sample"; break;
        case 0b111: i_bitrate = "32 bits per sample"; break;
    }
    std::cout << std::endl;
    std::cout << "---- [Frame infos] ----" << std::endl;
    std::cout << "[https://www.ietf.org/archive/id/draft-ietf-cellar-flac-12.txt]" << std::endl;
    std::cout << "   - Sync    : " << std::bitset<8>(m_sync_code) << "  |" << std::endl;
    std::cout << "   - Strat   : " << std::bitset<1>(m_blocking_strategy) << "         | " << i_blocking_strategy << std::endl;
    std::cout << "   - Size    : " << std::bitset<4>(m_blocksize) << "      | " << i_blocksize << std::endl;
    std::cout << "   - Sample  : " << std::bitset<4>(m_samplerate) << "      | " << i_samplerate << std::endl;
    std::cout << "   - Channel : " << std::bitset<4>(m_channels) << "      | " << i_channels << std::endl;
    std::cout << "   - Bitrate : " << std::bitset<3>(m_bitrate) << "       | " << i_bitrate << std::endl;
    std::cout << "-------- [End] --------" << std::endl << std::endl;
}
