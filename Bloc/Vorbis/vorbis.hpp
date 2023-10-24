#ifndef VORBIS_HPP
#define VORBIS_HPP

#include "../bloc.hpp"

class VorbisBloc : private Bloc {
//using Bloc::Bloc;
public:
	VorbisBloc(std::shared_ptr<Bloc> bloc);
    void read_vorbis();
    void print_vorbis();
    const uint8_t TYPE = 4;
	using Bloc::is_last;
	
private:
	std::shared_ptr<Bloc> m_parent_bloc;

    uint64_t m_vendor_length = -1;
	uint64_t m_comment_length = -1;
	uint64_t m_user_comment_list_length = -1;

	std::string m_vendor_string = "";
	std::string m_comment_string = "";
	std::vector<std::string> m_user_comment_list = {};
};

#endif // VORBIS_HPP