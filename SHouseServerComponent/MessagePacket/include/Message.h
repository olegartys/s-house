//
// Created by olegartys on 31.03.16.
//

#ifndef QUERYANALYZINGPACKET_MESSAGE_H
#define QUERYANALYZINGPACKET_MESSAGE_H


#include <stddef.h>
#include <string>
#include <json/json.h>

#include "ISerializable.h"

class Message: public ISerializable {
public:
    enum class QueryType { GET_STATE, SET_STATE };
    enum class SensorType { MONITORING, MULTIPLE, BINARY };

    Message(const std::string& src);

protected:
    virtual std::string serialize() const;

    virtual ISerializable::ReturnCode deserialize(const std::string &src);

protected:
    size_t id;
    QueryType queryType;

    std::string sensorName;
    std::string sensorType;
    std::string data;

};


#endif //QUERYANALYZINGPACKET_MESSAGE_H
