#ifndef VORBIS_HPP
#define VORBIS_HPP

#include "../bloc.hpp"

class VorbisBloc : private Bloc {
using Bloc::Bloc;
public:
    void read_vorbis();
    void print_vorbis();
private:
    std::string read_string(uint64_t length);

    uint64_t m_vendor_length = 0;
	uint64_t m_comment_length = 0;
	uint64_t m_user_comment_list_length = 0;

	std::string m_vendor_string = "";
	std::string m_comment_string = "";
	std::vector<std::string> m_user_comment_list = {};

    std::string capitalize(const std::string& s) {
        std::string sout = s;
        for (size_t i = 1; i < sout.length(); i++) {
            sout[i] = tolower(s[i]);
        }
        sout[0] = toupper(s[0]);
        return sout;
    }
};

#endif // VORBIS_HPP