#include "vorbis.hpp"
#include "../../Utils/utils.hpp"

VorbisBloc::VorbisBloc(std::shared_ptr<Bloc> bloc) {
	this->m_inp = bloc.get()->get_inp();
	this->m_last = bloc.get()->is_last();
	this->m_type = bloc.get()->get_type();
	this->m_length = bloc.get()->get_length();
	this->m_parent_bloc = bloc;
}

void VorbisBloc::read_vorbis() {
    this->read_header();
	this->m_parent_bloc.get()->update_bloc(this->m_last,this->m_type,this->m_length);
    assert(this->m_type == TYPE);
    // Get vendor length
    m_vendor_length = this->read_size(32, Order::LITTLE_ENDIAN_ORDER);
	// Get vendor string
    m_vendor_string = this->read_string(m_vendor_length);
	// Get comment list length
    m_user_comment_list_length = this->read_size(32, Order::LITTLE_ENDIAN_ORDER);
    for (uint64_t i = 0; i < m_user_comment_list_length; i++) {
		// Get comment length
        m_comment_length = this->read_size(32, Order::LITTLE_ENDIAN_ORDER);
		// Get comment string
        m_comment_string = this->read_string(m_comment_length);
		// Push comment to list
        m_user_comment_list.push_back(m_comment_string);
    }
}

void VorbisBloc::print_vorbis() {
    std::string comment;
	std::string comment_head;
	std::string comment_tail;

    std::cout << std::endl;
	std::cout << "Music infos :" << std::endl;
	std::cout << "    - Vendor      : " << m_vendor_string << std::endl;
	std::cout << "    - Comments    : " << std::endl;
	for (uint32_t i = 0; i < m_user_comment_list_length; ++i) {
		comment = m_user_comment_list[i];
		comment_head = comment.substr(0, comment.find('='));
		comment_tail = comment.substr(comment.find('=')+1, comment.length()-1);
		comment_head = capitalize(comment_head);
		if (comment_head == "Tracknumber") {
			comment_head = "Track";
		}
		std::cout << "        - " << std::setw(8) << std::left << std::setfill(' ') << comment_head << ": " << comment_tail << std::endl;
	}
	std::cout << std::endl;
}