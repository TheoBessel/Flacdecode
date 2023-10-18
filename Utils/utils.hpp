#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

std::string capitalize(const std::string& s) {
    std::string sout = s;
    for (size_t i = 1; i < sout.length(); i++) {
        sout[i] = tolower(s[i]);
    }
    sout[0] = toupper(s[0]);
    return sout;
}

#endif // UTILS_HPP