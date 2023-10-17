#ifndef METADATA_HPP
#define METADATA_HPP

#include "../bloc.hpp"

class MetadataBloc : private Bloc {
using Bloc::Bloc;
public:
    void read_metadata();
    void print_metadata();
private:
    uint64_t m_samplerate = -1;
	uint64_t m_numchannels = -1;
	uint64_t m_sampledepth = -1;
	uint64_t m_numsamples = -1;
    uint64_t m_time = -1;
};

#endif // METADATA_HPP