#ifndef PADDING_HPP
#define PADDING_HPP

#include "../bloc.hpp"

class PaddingBloc : private Bloc {
//using Bloc::Bloc;
public:
    PaddingBloc(std::shared_ptr<Bloc> bloc);
    void read_padding();
    const uint8_t TYPE = 1;
    using Bloc::is_last;
    
private:
    std::shared_ptr<Bloc> m_parent_bloc;
};

#endif // PADDING_HPP