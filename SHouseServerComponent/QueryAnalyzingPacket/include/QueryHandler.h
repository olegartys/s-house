//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_IQUERYHANDLER_H
#define QUERYANALYZINGPACKET_IQUERYHANDLER_H

#include <memory>
#include "IListener.h"
#include "IActor.h"
#include "../../CommonPacket/include/FunctionTraits.h"
#include "IHandler.h"


/**
 * @brief This class is sample implementation of IHandler interface.
 */
class QueryHandler : public IHandler {
public:

    explicit QueryHandler(Ptr<IListener> listener, Ptr<IActor> actor, Ptr<IDataStorage> storage);

    virtual void startListen() override;

    virtual Ptr<IActor> getActor()  override;
    virtual Ptr<IListener> getListener() override;
    virtual Ptr<IDataStorage> getStorage() override;

private:
    // Callbacks for IListener
    void onListenerNewMsg(std::string data);
    void onListenerError(std::string&& error);

    // Actor callbacks
    void onTransactionSuccess(Response msg);
    void onTransactionError(std::string err);

};

#endif //QUERYANALYZINGPACKET_IQUERYHANDLER_H
