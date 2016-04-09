//
// Created by olegartys on 27.03.16.
//

#include <iostream>
#include <event2/buffer.h>
#include <unistd.h>
#include <thread>
#include <Utils.h>
#include "QueryListener.h"

std::string QueryListener::TAG = "QueryListener";

IListener::ReturnCode QueryListener::init(IListener::OnNewQueryCatchedCallbackType onNew,
                                          IListener::OnErrorCatchedCallbackType onError, const IHandler *handler) {

    // Check wether callback are null
    if (onNew == nullptr || onError == nullptr) {
        return ReturnCode::NULL_CALLBACK;
    }

    if (handler != nullptr) {
        this->handler = handler;
    }

    this->onNew = onNew;
    this->onError = onError;

    // Set object to an initialized state
    this->_is_initialized = true;

    return ReturnCode::SUCCESS;
}


QueryListener::~QueryListener() {
    evconnlistener_free(connListener);
    event_base_free(eventBase);
}


QueryListener::ReturnCode QueryListener::listen() {

    // if _is__initialized is not set
    if (!_is_initialized) {
        return ReturnCode::NOT_INIT;
    }

    // TODO: handle errors
    // Initialize master socket
    int ret = initMasterSocket();
    if (ret < 0) {
        perror("initMasterSocket");
    }

    return ReturnCode::SUCCESS ;
}

const IHandler* QueryListener::getHandler() {
    return handler;
}


IListener::ReturnCode QueryListener::sendResponse(const Response &response) {
    std::cout << "[Response] " << response.serialize(true);
    return ReturnCode::SUCCESS ;
}


int QueryListener::initMasterSocket() {
    // Initialize master socket
    masterSock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (masterSock < 0) {
        LogD(TAG, "socket creation error");
        return -1;
    }

    masterAddr.sun_family = AF_UNIX;
    strcpy(masterAddr.sun_path, "/tmp/sock");

    // Init event base
    eventBase = event_base_new();
    if (eventBase == nullptr) {
        LogD(TAG, "event base init error");
        return -1;
    }

    // Create listener on a new connections
    connListener = evconnlistener_new_bind(eventBase, onAccept, (void*)this,
                                           (LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE), -1,
                                           (sockaddr*)&masterAddr, sizeof(masterAddr));

    if (connListener == nullptr) {
        LogD(TAG, "evconnlistener init error");
        return -1;
    }

    // Set callback on accepting error
    evconnlistener_set_error_cb(connListener, onAcceptError);

    // Start listening
    LogD(TAG, "Start listening");
    int ret = event_base_dispatch(eventBase);
}


void QueryListener::onAccept(evconnlistener *listener, evutil_socket_t fd, sockaddr *addr, int sock_len, void *arg) {
    LogD(TAG, "Accepted new connection with fd=" + std::to_string(fd));
    // When a new connection caught, connect it with bufferevent
    event_base* base = evconnlistener_get_base(listener);
    bufferevent* bufferevent = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE); // TODO: memory leak

    // Connect read, write and event callbacks to bufferevent of the connection
    bufferevent_setcb(bufferevent, onRead, onWrite, onEvent, arg);
    bufferevent_enable(bufferevent, (EV_READ | EV_WRITE));
}


void QueryListener::onAcceptError(evconnlistener *listener, void *arg) {
    LogD(TAG, "accept error");
}


void QueryListener::onRead(bufferevent *bufEvent, void *arg) {
    // Cast arg to QueryListener object
    QueryListener* _this = static_cast<QueryListener*>(arg);

    // Get the input from socket
    evbuffer *bufInput = bufferevent_get_input(bufEvent);

    // Convert it to the string
    std::string data = evBufferToString(bufInput);

    // Trigger callback
    std::thread thr(_this->onNew, data);
    thr.detach();

}


void QueryListener::onWrite(bufferevent *bufEvent, void *arg) {
    LogD(TAG, "Ready for write");
}


void QueryListener::onEvent(bufferevent *bufEvent, short events, void *arg) {
    LogD(TAG, "Event on master socket");
}


std::string QueryListener::evBufferToString(evbuffer *pEvbuffer) {
    size_t len = evbuffer_get_length(pEvbuffer);
    char* data = new char[len+1];
    memset(data, '\0', len+1);
    evbuffer_remove(pEvbuffer, data, len);
    std::string res(data);
    delete[] data;
    return res;
}
