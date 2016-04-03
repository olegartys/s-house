//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_ILISTENER_H
#define QUERYANALYZINGPACKET_ILISTENER_H

#include <functional>
#include "IHandler.h"

class IHandler;

/**
 * @brief This class represents interface of listener.
 * Listener is an object that catching messages from a client or user application.
 * Handler uses listener object for catching messages.
 * Normally handler is the owner of the listener.
 */
class IListener {
public:
    /**
     * Callback types.
     * Callback can transfer handler object if it is necessary for calling side.
     */
    using OnNewQueryCatchedCallbackType = std::function<void(std::string data)>;
    using OnErrorCatchedCallbackType    = std::function<void(std::string&& error)>;

    virtual ~IListener() = default;

    /**
     * @brief On this method listening is started (interface doesn't specifies rather block or nonblock method it is).
     * @param Handler handler who will own this object. From him connection to the surrounded world can be accessed.
     * @param OnNewQueryCatchedCallbackType callback that triggered when new message was catched.
     * @param OnErrorCatchedCallbackType callback that triggered when error was catched.
     */
    virtual int listen(OnNewQueryCatchedCallbackType, OnErrorCatchedCallbackType, const IHandler* = nullptr) = 0;

    /**
     * @brief this method returns handler associated with the listener.
     */
    virtual const IHandler* getHandler() = 0;

protected:
    const IHandler* handler;

};

#endif //QUERYANALYZINGPACKET_ILISTENER_H
