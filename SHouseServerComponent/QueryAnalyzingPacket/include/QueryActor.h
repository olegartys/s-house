//
// Created by olegartys on 02.04.16.
//

#ifndef SHOUSESERVERCOMPONENT_QUERYACTOR_H
#define SHOUSESERVERCOMPONENT_QUERYACTOR_H


#include <IActor.h>
#include "../../DataStoragePacket/include/IDataStorage.h"

class QueryActor: public IActor {

public:
    virtual ReturnCode init(const std::shared_ptr<IDataStorage> storage, OnSuccessCallbackType, OnErrorCallbackType) override;

    virtual ReturnCode commit(const Message &message) override;

private:
    OnSuccessCallbackType onSuccess;
    OnErrorCallbackType onError;

    std::shared_ptr<IDataStorage> storage;

    int onGetStateSuccess(int id, std::string data);
    int onGetStateError(std::string error);

};


#endif //SHOUSESERVERCOMPONENT_QUERYACTOR_H
