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
#include "ConnectionWrapper.h"

/**
 * @brief This class is sample implementation of IListener interface.
 * //TODO: asynchronous calls in a thread pool; sendResponse method
 *
 */
class QueryListener : public IListener {
public:

    /**
     * @brief constructs object. Does nothing but variables initialization and verification.
     * @param socketPath is the full path (with the name) of the socket which will be used for communication.
     * @param userAuthKey is the password which will be used to authenticate User communication component.
     * @param clientAuthKey is the password which will be used to authenticate Client communication component.
     */
    QueryListener(std::string socketPath, std::string userAuthKey, std::string clientAuthKey);

    /**
     * @brief free's the libevent resources.
     */
    ~QueryListener();

    /**
     * TODO
     */
    virtual ReturnCode sendResponse(const Response &response);

    /**
     * @brief this method is used to set callbacks to the object. It should be called after the constructor to be able to work with the object.
     * @param onNew is triggered when the new message has come.
     * @param onError is triggered when something went bad.
     * @return return the result of initialization (look at the constants in ReturnCode enum). //FIXME: how to insert doxygen reference?
     */
    virtual ReturnCode init(OnNewQueryCatchedCallbackType onNew, OnErrorCatchedCallbackType onError,
                            const IHandler *iHandler) override;

    /**
     * @brief starts listening. This method works in blocking mode, but all the callbacks will be called asynchronously.
     */
    virtual ReturnCode listen() override;

    /**
     *
     */
    virtual const IHandler* getHandler() override;

private:
    /**
     *
     */
    using SharedConnWrapper = std::shared_ptr<ConnectionWrapper>;

    /**
     * @brief
     */
    std::map<bufferevent *, SharedConnWrapper> connections;

private:

    /**
     * Path to the socket.
     */
    std::string socketPath;

    /**
     * User authentication key.
     */
    std::string userAuthKey;

    /**
     * Client authentication key.
     */
    std::string clientAuthKey;

    /**
     *
     */
    OnNewQueryCatchedCallbackType onNew;

    /**
     *
     */
    OnErrorCatchedCallbackType onError;

    // libevent routines
    /**
     * Master socket.
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

    const static std::string TAG;

    /**
     *
     */
    int initMasterSocket();

    // libevent callbacks
    /*
     * Triggers when new connection is accepted.
     */
    static void onAccept(evconnlistener* listener, evutil_socket_t fd, sockaddr* addr, int sock_len, void* arg);

    /**
     * Triggers when error catched during accept.
     */
    static void onAcceptError(evconnlistener* listener, void* arg);

    /**
     * Triggers when socket is ready for read.
     */
    static void onRead(bufferevent* bufEvent, void* arg);

    /**
     * Triggers when socket is ready for write.
     */
    static void onWrite(bufferevent* bufEvent, void* arg);

    /**
     * Triggers when some event socket.
     */
    static void onEvent(bufferevent* bufEvent, short events, void* arg);

    /**
     * Transfer buffer into a string.
     */
    static std::string evBufferToString(evbuffer *pEvbuffer);

};


#endif //QUERYANALYZINGPACKET_QUERYLISTENER_H
