#ifndef VORBIS_HPP
#define VORBIS_HPP

#include "../bloc.hpp"

class VorbisBloc : private Bloc {
using Bloc::Bloc;
public:
    void read_vorbis();
    void print_vorbis();
    const uint8_t TYPE = 4;
private:
    uint64_t m_vendor_length = -1;
	uint64_t m_comment_length = -1;
	uint64_t m_user_comment_list_length = -1;

	std::string m_vendor_string = "";
	std::string m_comment_string = "";
	std::vector<std::string> m_user_comment_list = {};
};

#endif // VORBIS_HPP