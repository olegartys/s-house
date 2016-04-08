//
// Created by olegartys on 27.03.16.
//

#include <iostream>
#include "QueryListener.h"

IListener::ReturnCode QueryListener::init(IListener::OnNewQueryCatchedCallbackType onNew,
                                          IListener::OnErrorCatchedCallbackType onError, const IHandler *handler) {

    if (onNew == nullptr || onError == nullptr) {
        return ReturnCode::NULL_CALLBACK;
    }

    if (handler != nullptr) {
        this->handler = handler;
    }

    this->onNew = onNew;
    this->onError = onError;

    this->_is_initialized = true;

    return ReturnCode::SUCCESS;
}

QueryListener::ReturnCode QueryListener::listen() {

    if (!_is_initialized) {
        return ReturnCode::NOT_INIT;
    }

    std::string s("{\n"
                          "  \"id\": 1,\n"
                          "  \"query_type\": 0,\n"
                          "  \"sensor_type\": 0,\n"
                          "  \"sensor_name\": \"kek\",\n"
                          "  \"data\": \"kek\"\n"
                          "}");
    onNew(s);
    onError("onErrorTest");

    return ReturnCode::SUCCESS ;
}

const IHandler* QueryListener::getHandler() {
    return handler;
}


IListener::ReturnCode QueryListener::sendResponse(const Response &response) {
    std::cout << "[Response] " << response.serialize(true);
    return ReturnCode::SUCCESS ;
}
