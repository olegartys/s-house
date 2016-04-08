//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_QUERYLISTENER_H
#define QUERYANALYZINGPACKET_QUERYLISTENER_H


#include "IListener.h"

/**
 * @brief This class is sample implementation of IListener interface.
 *
 */
class QueryListener : public IListener {
public:

    virtual ReturnCode init(OnNewQueryCatchedCallbackType type, OnErrorCatchedCallbackType callbackType,
                            const IHandler *iHandler) override;

    virtual ReturnCode listen() override;

    virtual const IHandler* getHandler() override;

private:
    OnNewQueryCatchedCallbackType onNew;
    OnErrorCatchedCallbackType onError;

};


#endif //QUERYANALYZINGPACKET_QUERYLISTENER_H
