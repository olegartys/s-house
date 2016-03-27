//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_FUNCTIONTRAITS_H
#define QUERYANALYZINGPACKET_FUNCTIONTRAITS_H

#include <tuple>
#include <functional>

template<typename T>
struct FunctionTraits;

template<typename R, typename ...Args>
struct FunctionTraits<std::function<R(Args...)>>
{
    static const size_t nargs = sizeof...(Args);

    typedef R result_type;

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};


#endif //QUERYANALYZINGPACKET_FUNCTIONTRAITS_H
