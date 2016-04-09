//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_QUERYLISTENER_H
#define QUERYANALYZINGPACKET_QUERYLISTENER_H


#include <event2/util.h>
#include <event2/listener.h>
#include <event2/event_struct.h>
#include <event2/event.h>
#include <event2/bufferevent_struct.h>

#include <sys/un.h>
#include "IListener.h"

/**
 * @brief This class is sample implementation of IListener interface.
 *
 */
class QueryListener : public IListener {
public:

    ~QueryListener();

    virtual ReturnCode sendResponse(const Response &response);

    virtual ReturnCode init(OnNewQueryCatchedCallbackType type, OnErrorCatchedCallbackType callbackType,
                            const IHandler *iHandler) override;

    virtual ReturnCode listen() override;

    virtual const IHandler* getHandler() override;

private:

    struct ConnWrapper {

        ConnWrapper() :
                fd(-1), bufEvent(nullptr) {}

        ~ConnWrapper() { bufferevent_free(bufEvent); }

        bool isInit() const { return bufEvent != nullptr; }

        evutil_socket_t fd;
        bufferevent* bufEvent;
    };

    ConnWrapper userConn;
    ConnWrapper clientConn;

    /**
     *
     */
    OnNewQueryCatchedCallbackType onNew;

    /**
     *
     */
    OnErrorCatchedCallbackType onError;

    // For socket connection
    /**
     *
     */
    evutil_socket_t masterSock;

    /**
     *
     */
    sockaddr_un masterAddr;

    /**
     *
     */
    event_base* eventBase;

    /**
     *
     */
    evconnlistener* connListener;

private:
    /**
     *
     */
    static std::string TAG;

    /**
     *
     */
    int initMasterSocket();

    /*
     * libevent callbacks
     */
    static void onAccept(evconnlistener* listener, evutil_socket_t fd, sockaddr* addr, int sock_len, void* arg);

    /**
     *
     */
    static void onAcceptError(evconnlistener* listener, void* arg);

    // TODO добавить аутентификацию нужных клиентов (а то кто угодно сможет слать сообщения)
    /**
     *
     */
    static void onRead(bufferevent* bufEvent, void* arg);

    /**
     *
     */
    static void onWrite(bufferevent* bufEvent, void* arg);

    /**
     *
     */
    static void onEvent(bufferevent* bufEvent, short events, void* arg);

    static std::string evBufferToString(evbuffer *pEvbuffer);
};


#endif //QUERYANALYZINGPACKET_QUERYLISTENER_H
