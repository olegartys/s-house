//
// Created by olegartys on 27.03.16.
//

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

    onNew("onNewTest");
    onError("onErrorTest");

    return ReturnCode::SUCCESS ;
}

const IHandler* QueryListener::getHandler() {
    return handler;
}


