#include "padding.hpp"

PaddingBloc::PaddingBloc(std::shared_ptr<Bloc> bloc) {
	this->m_inp = bloc.get()->get_inp();
	this->m_last = bloc.get()->is_last();
	this->m_type = bloc.get()->get_type();
	this->m_length = bloc.get()->get_length();
	this->m_parent_bloc = bloc;
}

void PaddingBloc::read_padding() {
    this->read_header();
	this->m_parent_bloc.get()->update_bloc(this->m_last,this->m_type,this->m_length);
    // Skip informations
    this->print_info();
    this->read_body();
}