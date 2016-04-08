//
// Created by olegartys on 02.04.16.
//

#include "QueryActor.h"

IActor::ReturnCode QueryActor::init(const std::shared_ptr<IDataStorage> storage, OnSuccessCallbackType onSuccess, OnErrorCallbackType onError) {

    if (onSuccess == nullptr || onError == nullptr) {
        return ReturnCode::NULL_CALLBACK;
    }

    this->onSuccess = onSuccess;
    this->onError = onError;

    this->_is_initialized = true;

    return ReturnCode::SUCCESS;
}

IActor::ReturnCode QueryActor::commit(const Message &message) {
    if (!_is_initialized) {
        return ReturnCode::NOT_INIT;
    }

    switch (message.queryType) {
        case Message::QueryType::GET_STATE:

            break;

        case Message::QueryType::SET_STATE:

            break;

        case Message::QueryType::DEPRECATED_QUERY:
        default:

            break;
    }

    return ReturnCode::SUCCESS;
}
