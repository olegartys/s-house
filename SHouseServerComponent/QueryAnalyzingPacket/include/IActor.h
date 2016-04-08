//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_IACTOR_H
#define QUERYANALYZINGPACKET_IACTOR_H

#include <Message.h>
#include <functional>
#include "../../DataStoragePacket/include/IDataStorage.h"
#include <Response.h>

class IActor {
public:

    using OnSuccessCallbackType  = std::function<void(Response data)>;
    using OnErrorCallbackType    = std::function<void(Response error)>;

    virtual ~IActor() = default;

    enum class ReturnCode: int {
        SUCCESS,
        NULL_CALLBACK,
        NOT_INIT,
        NULL_STORAGE
    };

    virtual ReturnCode init(const std::shared_ptr<IDataStorage>, OnSuccessCallbackType, OnErrorCallbackType) = 0;

    virtual ReturnCode commit(const Message&) = 0;

protected:
    bool _is_initialized = false;

};

#endif //QUERYANALYZINGPACKET_IACTOR_H
