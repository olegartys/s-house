//
// Created by olegartys on 02.04.16.
//

#ifndef QUERYANALYZINGPACKET_ISERIALIZABLE_H
#define QUERYANALYZINGPACKET_ISERIALIZABLE_H

#include <string>

class ISerializable {
public:
    virtual ~ISerializable() = default;
    enum class ReturnCode { SUCCESS, DESERIALIZE_ERR };

protected:
    virtual std::string serialize() const = 0;
    virtual ReturnCode deserialize(const std::string& src) = 0;

};

#endif //QUERYANALYZINGPACKET_ISERIALIZABLE_H
