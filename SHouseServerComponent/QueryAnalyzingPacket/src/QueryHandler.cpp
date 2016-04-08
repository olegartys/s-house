//
// Created by olegartys on 27.03.16.
//

#include <iostream>
#include <Message.h>
#include "Utils.h"
#include "QueryHandler.h"

QueryHandler::QueryHandler(Ptr<IListener> listener, Ptr<IParser> parser, Ptr<IActor> actor, Ptr<IDataStorage> storage)
{
    this->listener = listener;
    this->parser = parser;
    this->actor = actor;
    this->storage = storage;
}


void QueryHandler::startListen() {
//    int res = listener->listen(std::bind(&QueryHandler::onListenerNewMsg, this, std::placeholders::_1),
//                               std::bind(&QueryHandler::onListenerError, this, std::placeholders::_1));
//    if (res < 0) {
//        // Error
//    }
}

/*---------------------------------------------Listener callbacks-----------------------------------------------------*/
void QueryHandler::onListenerNewMsg(std::string data) {
//    std::cout << data << std::endl;
//    this->parser->parse(data, std::bind(&QueryHandler::onParserSuccess, this, std::placeholders::_1),
//                        std::bind(&QueryHandler::onParserError, this, std::placeholders::_1));
    // Parse message
    Message::ReturnCode res;
    Message msg(data, res);
    switch (res) {
        // If message parsed successfully
        case Message::ReturnCode::SUCCESS:

            break;

            // Get the wrong format or broken message
        case Message::ReturnCode::DESERIALIZE_ERR:

            break;
    }
};


void QueryHandler::onListenerError(std::string&& error) {
    std::cout << error << std::endl;
}
/*--------------------------------------------------------------------------------------------------------------------*/



IHandler::Ptr<IListener> QueryHandler::getListener() {
    return listener;
}

IHandler::Ptr<IParser> QueryHandler::getParser() {
    return parser;
}

IHandler::Ptr<IActor> QueryHandler::getActor() {
    return actor;
}

IHandler::Ptr<IDataStorage> QueryHandler::getStorage() {
    return storage;
}
