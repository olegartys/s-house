//
// Created by olegartys on 27.03.16.
//

#include <iostream>
#include <Message.h>
#include "Utils.h"
#include "QueryHandler.h"

QueryHandler::QueryHandler(Ptr<IListener> listener, Ptr<IActor> actor, Ptr<IDataStorage> storage)
{
    this->listener = listener;
    this->actor = actor;
    this->storage = storage;
}


void QueryHandler::startListen() {
    // Init listener
    IListener::ReturnCode res = listener->init(std::bind(&QueryHandler::onListenerNewMsg, this, std::placeholders::_1),
                           std::bind(&QueryHandler::onListenerError, this, std::placeholders::_1));

    switch (res) {

        case IListener::ReturnCode::SUCCESS:
            break;

        case IListener::ReturnCode::NULL_CALLBACK:
            break;

        case IListener::ReturnCode::NOT_INIT:
            break;
    }

    // Init actor
    IActor::ReturnCode res1 = actor->init(storage, std::bind(&QueryHandler::onTransactionSuccess, this, std::placeholders::_1),
                                          std::bind(&QueryHandler::onTransactionError, this, std::placeholders::_1));

    switch (res1) {

        case IActor::ReturnCode::SUCCESS:

            break;

        case IActor::ReturnCode::NULL_CALLBACK:

            break;

        case IActor::ReturnCode::NOT_INIT:

            break;

        case IActor::ReturnCode::NULL_STORAGE:

            break;
    }

    listener->listen();
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
            LogD("KEK", "HERE");
            actor->commit(msg); //TODO check return code

            break;

        // Get the wrong format or broken message
        case Message::ReturnCode::DESERIALIZE_ERR:
            LogD("KEK", "ERROR");
            break;
    }
};


void QueryHandler::onListenerError(std::string&& error) {
    std::cout << error << std::endl;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------Actor callbacks--------------------------------------------------------*/
void QueryHandler::onTransactionSuccess(Response msg) {
    listener->sendResponse(msg);
}

void QueryHandler::onTransactionError(std::string err) {

}

/*--------------------------------------------------------------------------------------------------------------------*/


IHandler::Ptr<IListener> QueryHandler::getListener() {
    return listener;
}


IHandler::Ptr<IActor> QueryHandler::getActor() {
    return actor;
}

IHandler::Ptr<IDataStorage> QueryHandler::getStorage() {
    return storage;
}


