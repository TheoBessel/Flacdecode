#ifndef FRAME_HPP
#define FRAME_HPP

#include "../BitInput/bitinput.hpp"
#include <iostream>
#include <array>

const std::array<std::vector<int64_t>, 5> FIXED_PREDICTION_COEFFICIENTS = {
    {
        {},
        {1},
        {2,-1},
        {3,-3,1},
        {4,-6,4,-1}
    }
};

class Frame {
public:
    Frame(std::shared_ptr<BitInput> inp, const uint64_t& bitrate);
    void read_header();
    void read_frame();
    
    std::vector<std::vector<int64_t> > read_subframes();
    std::vector<int64_t> read_subframe(const uint64_t& bitrate);
    std::vector<int64_t> fixed_prediction(const uint64_t& prediction_order, const uint64_t& bitrate);
    std::vector<int64_t> linear_prediction(const uint64_t& prediction_order, const uint64_t& bitrate);

    void compute_residuals(std::vector<int64_t> *subframe);

    void restore_prediction(std::vector<int64_t> *subframe, const std::vector<int64_t>& coefficients, const u_int64_t& shift);

    void read_footer();
    void print_info();

    bool is_last() { return (m_inp->read_octet(false) == -3); }

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