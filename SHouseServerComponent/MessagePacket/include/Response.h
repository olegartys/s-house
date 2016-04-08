//
// Created by olegartys on 08.04.16.
//

#ifndef SHOUSESERVERCOMPONENT_RESPONSE_H
#define SHOUSESERVERCOMPONENT_RESPONSE_H


#include "ISerializable.h"

struct Response: public ISerializable {

    Response() = default;
    Response(const std::string& src, ReturnCode& res);

    virtual ReturnCode deserialize(const std::string& src) override;
    virtual std::string serialize(bool styled) const override;

    struct StatusCode {
        enum { SUCCESS };
    };

    int id;
    int statusCode;
    std::string data;

};


#endif //SHOUSESERVERCOMPONENT_RESPONSE_H
