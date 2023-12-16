#pragma once
#ifndef __SRG_HELPER_TEMPLATES_HEADER__
#define __SRG_HELPER_TEMPLATES_HEADER__

#include "__defs.hpp"
#include <chrono>

#define DEBUG(S) UtilityFunctions::print(S)

using grid_t = std::vector<std::vector<int64_t>>;

inline std::string translate(const String s) { 
    return std::string(s.utf8().get_data()); 
};
inline String translate(const std::string& s) {
    return String(s.data());
};

inline Array split(const String source, const String separator) {
    Array a = source.split(separator);
    Array result;
    for(int i = 0; i < a.size(); i++) {
        String item = a[i];
        result.append(item.strip_edges());
    }
    return result;
};

template<typename T>
inline Array translate(const std::vector<T>& source) {
    Array a;
    for (auto item : source) {
        a.append(item);
    }
    return a;
};
template<typename T>
inline std::vector<T> translate(Array source) {
    std::vector<T> result;
    for(int i = 0; i < source.size(); i++) {
        T item = static_cast<T>(source[i]);
        result.push_back(item);
    }
    return result;
};

const std::string WHITESPACE = " \n\r\t";

inline std::string ltrim(const std::string& s)
{
    size_t startpos = s.find_first_not_of(WHITESPACE);
    return (startpos == std::string::npos) ? "" : s.substr(startpos);
}

inline std::string rtrim(const std::string& s)
{
    size_t endpos = s.find_last_not_of(WHITESPACE);
    return (endpos == std::string::npos) ? "" : s.substr(0, endpos+1);
}

inline std::string trim(const std::string& s)
{
    return rtrim(ltrim(s));
}

#include <queue>

template<typename T,class _Container = std::vector<T>, class _Pr = std::less<typename _Container::value_type>>
class custom_priority_queue : public std::priority_queue<T, _Container, _Pr>
{
public:

    explicit custom_priority_queue(const _Pr& _Pred) : std::priority_queue<T, _Container, _Pr>(_Pred) {
    }

    bool remove(const T& value) {
        auto it = std::find(this->c.begin(), this->c.end(), value);

        if (it == this->c.end()) {
            return false;
        }
        if (it == this->c.begin()) {
            // deque the top element
            this->pop();
        }
        else {
            // remove element and re-heap
            this->c.erase(it);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
        }
        return true;
    }
};


#endif /// __SRG_HELPER_TEMPLATES_HEADER__
