//
// Created by olegartys on 31.03.16.
//

#include <Message.h>

Message::Message(const std::string &src) {
    ISerializable::ReturnCode code = this->deserialize(src);
    switch (code) {
        case ISerializable::ReturnCode::DESERIALIZE_ERR:
            break;

        case ISerializable::ReturnCode::SUCCESS:
            break;
    }
}


std::string Message::serialize() const {
    Json::Value root;
    root["kek"] = "kek";
    //JSon
    return std::string();
}


ISerializable::ReturnCode Message::deserialize(const std::string &src) {
    return ISerializable::ReturnCode();
}

