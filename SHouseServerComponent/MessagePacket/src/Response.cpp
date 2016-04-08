//
// Created by olegartys on 08.04.16.
//

#include <json/value.h>
#include <json/reader.h>
#include "Response.h"

std::string Response::serialize(bool styled) const {
    Json::Value root;

    root["id"] = id;
    root["status_code"] = statusCode;
    root["data"] = data;

    return styled ? root.toStyledString() : root.asString();
}

ISerializable::ReturnCode Response::deserialize(const std::string &src) {
    ReturnCode returnCode = ReturnCode::SUCCESS;

    try {
        // Init json object
        Json::Reader reader;
        Json::Value root;
        reader.parse(src, root);

        // Get ID
        int id = root["id"].asInt();
        if (id == -1) {
            returnCode = ReturnCode::DESERIALIZE_ERR;
        }
        this->id = id;

        // Get status_code
        int statusCode = root["status_code"].asInt();
        if (id == -1) {
            returnCode = ReturnCode::DESERIALIZE_ERR;
        }
        this->statusCode = statusCode;

        // Get data
        std::string data = root["data"].asString();
        this->data = data;

    } catch (std::exception&) {
        returnCode =  ReturnCode::DESERIALIZE_ERR;
    }

    return returnCode;
}

Response::Response(const std::string &src, ReturnCode& res) {
    res = this->deserialize(src);
}
