//
// Created by olegartys on 31.03.16.
//

#include <Message.h>
#include <iostream>


Message::Message(const std::string &src, ReturnCode& res) {
    res = this->deserialize(src);
}


std::string Message::serialize(bool styled) const {
    Json::Value root;

    root["id"] = id;
    root["query_type"] = queryType;
    root["sensor_name"] = sensorName;
    root["data"] = data;

    return styled ? root.toStyledString() : root.asString();
}


Message::ReturnCode Message::deserialize(const std::string &src) {

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

        // Get host
        std::string host = root["host"].asString();
        if (host == "") {
            returnCode = ReturnCode::DESERIALIZE_ERR;
        }
        this->host = host;

        // Get query_type
        int queryType = root["query_type"].asInt();
        if (queryType == QueryType::DEPRECATED_QUERY) {
            returnCode = ReturnCode::DESERIALIZE_ERR;
        }
        this->queryType = queryType;

        // Get sensor name
        std::string sensorName = root["sensor_name"].asString();
        if (sensorName == "") {
            returnCode = ReturnCode::DESERIALIZE_ERR;
        }
        this->sensorName = sensorName;

        // Get data
        std::string data = root["data"].asString();
        this->data = data;

    } catch (std::exception&) {
        returnCode =  ReturnCode::DESERIALIZE_ERR;
    }

    return returnCode;
}

