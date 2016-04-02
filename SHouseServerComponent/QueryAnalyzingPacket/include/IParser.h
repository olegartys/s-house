//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_IPARSER_H
#define QUERYANALYZINGPACKET_IPARSER_H

#include <functional>
#include "SensorInfo.h"

//template <typename SensorContainer>
class IParser {
public:
    using OnSuccess = std::function<void(SensorInfo)>;
    using OnError = std::function<void(std::string&& error)>;

    virtual int parse(const std::string&, OnSuccess, OnError) = 0;
};

#endif //QUERYANALYZINGPACKET_IPARSER_H
