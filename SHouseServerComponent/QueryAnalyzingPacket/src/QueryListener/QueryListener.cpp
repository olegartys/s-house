//
// Created by olegartys on 27.03.16.
//

#include <iostream>
#include <event2/buffer.h>
#include <thread>
#include "../../../CommonPacket/include/Utils.h"
#include <QueryListener/QueryListener.h>
#include <assert.h>
#include <unistd.h>

const std::string QueryListener::TAG = "QueryListener";

/*********************************************Interface implementation*************************************************/
QueryListener::QueryListener(std::string socketPath, std::string userAuthKey, std::string clientAuthKey) :
        socketPath(socketPath), userAuthKey(userAuthKey), clientAuthKey(clientAuthKey) {

    assert(userAuthKey != clientAuthKey);
}


QueryListener::~QueryListener() {
    if (connListener) evconnlistener_free(connListener);
    if (eventBase) event_base_free(eventBase);
}


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
        return ReturnCode::UNKNOWN_ERROR;
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


/*************************************************Logic implementation*************************************************/
int QueryListener::initMasterSocket() {
    // Initialize master socket
    masterSock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (masterSock < 0) {
        LogD(TAG, "socket creation error");
        throw std::runtime_error(strerror(errno));
    }

    masterAddr.sun_family = AF_UNIX;
    strcpy(masterAddr.sun_path, socketPath.c_str());

    // Init event base
    eventBase = event_base_new();
    if (eventBase == nullptr) {
        LogD(TAG, "event base init error");
        throw std::runtime_error("Event base init error.");
    }

    // Create listener on a new connections
    unlink(socketPath.c_str()); // delete socket if it exists
    connListener = evconnlistener_new_bind(eventBase, onAccept, (void*)this,
                                           (LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE), -1,
                                           (sockaddr*)&masterAddr, sizeof(masterAddr));

    if (connListener == nullptr) {
        LogD(TAG, "evconnlistener init error");
        throw std::runtime_error("Evconnlistener init error.");
        return -1;
    }

    // Set callback on accepting error
    evconnlistener_set_error_cb(connListener, onAcceptError);

    // Start listening
    LogD(TAG, "Start listening");
    int ret = event_base_dispatch(eventBase);
    return ret;
}


void QueryListener::onAccept(evconnlistener *listener, evutil_socket_t fd, sockaddr *addr, int sock_len, void *arg) {
    LogD(TAG, "Accepted new connection with fd=" + std::to_string(fd));
    // When a new connection caught, connect it with bufferevent
    event_base* base = evconnlistener_get_base(listener);
    bufferevent *bufferevent = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

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
    trim(data);

    evutil_socket_t fd = bufferevent_getfd(bufEvent);

    // Search for connection in the collection
    auto connection = _this->connections.find(bufEvent);

    // If connection contained in the collection
    if (connection != _this->connections.end()) {
        // Trigger success callback
        std::thread thr(_this->onNew, data);
        thr.detach();

        // If not...
    } else {

        // if data equals to one of the keys
        if (data == _this->userAuthKey || data == _this->clientAuthKey) {

            // and if data NOT equals to the existing connection (to prevent twice connection)...
            bool exists = false;
            for (auto it: _this->connections) {
                if (it.second->isKeyEqual(data)) {
                    exists = true;
                }
            }

            // ... so the authentication is over and we are adding new connection into collection
            if (!exists) {
                _this->connections[bufEvent] = SharedConnWrapper(new ConnectionWrapper(bufEvent, data));
                // else just close it
            } else {
                evutil_closesocket(fd);
            }
            // close socket if received data is not equal to auth messages
        } else {
            evutil_closesocket(fd);
        }
    }

}


void QueryListener::onWrite(bufferevent *bufEvent, void *arg) {
    LogD(TAG, "Ready for write");
}


void QueryListener::onEvent(bufferevent *bufEvent, short events, void *arg) {

    QueryListener *_this = static_cast<QueryListener *>(arg);
    evutil_socket_t fd = bufferevent_getfd(bufEvent);

    // If socket is closed - delete it from the collection
    if (events & BEV_EVENT_EOF) {
        LogD(TAG, "event: BEV_EVENT_EOF fd=" + std::to_string(fd));
        _this->connections.erase(bufEvent);
    }

    if (events & BEV_EVENT_ERROR) {
        LogD(TAG, "event: BEV_EVENT_ERROR fd=" + std::to_string(fd));
    }

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

