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
    virtual int listen(OnNewQueryCatchedCallbackType type,
                       OnErrorCatchedCallbackType callbackType,
                       const IHandler* handler = nullptr) override;

    virtual const IHandler* getHandler() override;

};


#endif //QUERYANALYZINGPACKET_QUERYLISTENER_H
