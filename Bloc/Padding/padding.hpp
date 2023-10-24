#ifndef PADDING_HPP
#define PADDING_HPP

#include "../bloc.hpp"

class PaddingBloc : private Bloc {
public:
    PaddingBloc(std::shared_ptr<Bloc> bloc);
    void read_padding();
    using Bloc::is_last;
    
private:
    std::shared_ptr<Bloc> m_parent_bloc;
};

#endif // PADDING_HPP