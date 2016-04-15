//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_UTILS_H
#define QUERYANALYZINGPACKET_UTILS_H

#include <string>
#include <memory>
#include <algorithm>

/*
 * Prints ERR message in std::cerr with TAG before it.
 */
void LogD(std::string TAG, std::string ERR);

/**
 * Just for clarity and beauty
 */
template<typename T> using Ptr = std::shared_ptr<T>;

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

#endif //QUERYANALYZINGPACKET_UTILS_H
