//
// Created by olegartys on 02.04.16.
//

#include "QueryActor.h"

IActor::ReturnCode QueryActor::init(const std::shared_ptr<IDataStorage> storage, OnSuccessCallbackType onSuccess, OnErrorCallbackType onError) {

    if (onSuccess == nullptr || onError == nullptr) {
        return ReturnCode::NULL_CALLBACK;
    }

    if (storage == nullptr) {
        return ReturnCode::NULL_STORAGE;
    }

    this->onSuccess = onSuccess;
    this->onError = onError;
    this->storage = storage;

    this->_is_initialized = true;

    return ReturnCode::SUCCESS;
}

IActor::ReturnCode QueryActor::commit(const Message &message) {
    if (!_is_initialized) {
        return ReturnCode::NOT_INIT;
    }

    switch (message.queryType) {
        case Message::QueryType::GET_STATE: //TODO handle return value
            storage->getState(message.sensorName, std::bind(&QueryActor::onGetStateSuccess, this, message.id, std::placeholders::_1),
                              std::bind(&QueryActor::onGetStateError, this, std::placeholders::_1));
            break;

        case Message::QueryType::SET_STATE:

            break;

        case Message::QueryType::DEPRECATED_QUERY:
        default:

            break;
    }

    return ReturnCode::SUCCESS;
}

int QueryActor::onGetStateSuccess(int id, std::string data) {
    Response resp;
    resp.id = id;
    resp.data = data;
    resp.statusCode = Response::StatusCode::SUCCESS;

    onSuccess(resp);
}

int QueryActor::onGetStateError(std::string error) {
    onError(error);
}
