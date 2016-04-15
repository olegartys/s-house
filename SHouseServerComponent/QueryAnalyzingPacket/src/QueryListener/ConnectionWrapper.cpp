//
// Created by olegartys on 09.04.16.
//

#include <QueryListener/ConnectionWrapper.h>
#include <assert.h>
#include "../../../CommonPacket/include/Utils.h"

const std::string ConnectionWrapper::TAG = "ConnectionWrapper";

ConnectionWrapper::ConnectionWrapper(bufferevent *bufEvent, const std::string &authKey) :
        bufEvent(bufEvent), authKey(authKey), fd(-1) {
    if (bufEvent != nullptr) {
        fd = bufferevent_getfd(bufEvent);
        LogD(TAG, "Connection established fd=" + std::to_string(fd));
    }
}

ConnectionWrapper::~ConnectionWrapper() {
    close();
}

void ConnectionWrapper::init(bufferevent *bufEvent) {
    assert(bufEvent != nullptr);
    this->bufEvent = bufEvent;
    this->fd = bufferevent_getfd(bufEvent);
    LogD(TAG, "Connection established fd=" + std::to_string(fd));
}

void ConnectionWrapper::close() {
    // This will close socket too only if flag BEV_OPT_CLOSE_ON_FREE is set
    if (bufEvent != nullptr) {
        bufferevent_free(bufEvent);
    }
    LogD(TAG, "Connection closed fd=" + std::to_string(fd));
}
