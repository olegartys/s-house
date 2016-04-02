//
// Created by olegartys on 31.03.16.
//

#include <Message.h>

Message::Message(const std::string &src) {
    this->deserialize(src);
}


std::string Message::serialize() const {
    Json::Value root;
    //JSon
    return std::string();
}


ISerializable::ReturnCode Message::deserialize(const std::string &src) {
    return ISerializable::ReturnCode();
}

