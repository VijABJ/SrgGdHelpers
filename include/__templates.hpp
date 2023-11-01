#pragma once
#ifndef __SRG_HELPER_TEMPLATES_HEADER__
#define __SRG_HELPER_TEMPLATES_HEADER__

#include "__defs.hpp"

#define DEBUG(S) UtilityFunctions::print(S)

using grid_t = std::vector<std::vector<int64_t>>;

inline std::string translate(const String s) { 
    return std::string(s.utf8().get_data()); 
}
inline String translate(const std::string& s) {
    return String(s.data());
}

template<typename T>
inline Array translate(const std::vector<T>& source) {
    Array a;
    for (auto item : source) {
        a.append(item);
    }
    return a;
};
template<typename T>
inline std::vector<T> translate(const Array source) {
    std::vector<T> result;
    for(int i = 0; i < source.size(); i++) {
        result.push_back(source[i]);
    }
    return result;
}

#endif /// __SRG_HELPER_TEMPLATES_HEADER__
