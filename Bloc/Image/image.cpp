#include "image.hpp"

ImageBloc::ImageBloc(std::shared_ptr<Bloc> bloc) {
	this->m_inp = bloc.get()->get_inp();
	this->m_last = bloc.get()->is_last();
	this->m_type = bloc.get()->get_type();
	this->m_length = bloc.get()->get_length();
    this->m_parent_bloc = bloc;
}

void ImageBloc::read_image() {
    this->read_header();
    this->m_parent_bloc.get()->update_bloc(this->m_last,this->m_type,this->m_length);
    assert(this->m_type == TYPE);
    // Get picture type
    m_picture_type = this->m_inp->read_uint(32);

    m_mime_length = this->read_size(32);
    m_mime_string = this->read_string(m_mime_length);
    
    m_desc_length = this->read_size(32);
    m_desc_string = this->read_string(m_desc_length);
    
    m_width = this->m_inp->read_uint(32);
    m_height = this->m_inp->read_uint(32);
    m_depth = this->m_inp->read_uint(32);
    m_channels = this->m_inp->read_uint(32);

    m_data_length = this->read_size(32);
    m_data_string = this->read_string(m_data_length);
}

void ImageBloc::print_image() {
    std::cout << std::endl;
	std::cout << "Picture infos :" << std::endl;
	std::cout << "    - Type      : " << m_picture_type << std::endl;
	std::cout << "    - Mime      : " << m_mime_string << std::endl;
	std::cout << "    - Desc      : " << m_desc_string << std::endl;
	std::cout << "    - Width     : " << m_width << std::endl;
	std::cout << "    - Height    : " << m_height << std::endl;
	std::cout << "    - Depth     : " << m_depth << std::endl;
	std::cout << "    - Channel   : " << m_channels << std::endl;
	std::cout << "    - Data size : " << m_data_length << std::endl;
 	std::cout << std::endl;
}