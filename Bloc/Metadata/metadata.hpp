#ifndef METADATA_HPP
#define METADATA_HPP

#include "../bloc.hpp"

class MetadataBloc : private Bloc {
//using Bloc::Bloc;
public:
    MetadataBloc(std::shared_ptr<Bloc> bloc);
    void read_metadata();
    void print_metadata();
    const uint8_t TYPE = 0;
    using Bloc::is_last;
    
private:
    std::shared_ptr<Bloc> m_parent_bloc;

    uint64_t m_samplerate = -1;
	uint64_t m_numchannels = -1;
	uint64_t m_sampledepth = -1;
	uint64_t m_numsamples = -1;
    uint64_t m_time = -1;
};

#endif // METADATA_HPP