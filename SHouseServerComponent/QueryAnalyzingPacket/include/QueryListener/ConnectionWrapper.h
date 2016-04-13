// Created by olegartys on 09.04.16.
//

#ifndef SHOUSESERVERCOMPONENT_CONNECTIONWRAPPER_H
#define SHOUSESERVERCOMPONENT_CONNECTIONWRAPPER_H

#include <event2/bufferevent.h>
#include <string>


class ConnectionWrapper final {
public:
    ConnectionWrapper(bufferevent *bufEvent, const std::string &key);

    ~ConnectionWrapper();

    void init(bufferevent *bufEvent);

    void close();

    bool isAuth() const { return bufEvent != nullptr; }

    bool isKeyEqual(const std::string &authKey) { return this->authKey == authKey; }

    evutil_socket_t getFd() const { return fd; }

private:
    std::string authKey;
    evutil_socket_t fd;
    bufferevent *bufEvent;

    static const std::string TAG;
};


#endif //SHOUSESERVERCOMPONENT_CONNECTIONWRAPPER_H
