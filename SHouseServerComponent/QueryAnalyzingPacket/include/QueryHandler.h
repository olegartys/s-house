//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_IQUERYHANDLER_H
#define QUERYANALYZINGPACKET_IQUERYHANDLER_H

#include <memory>
#include "IListener.h"
#include "IParser.h"
#include "IActor.h"
#include "FunctionTraits.h"
#include "IHandler.h"


/**
 * @brief This class is sample implementation of IHandler interface.
 */
class QueryHandler : public IHandler {
public:

    explicit QueryHandler(Ptr<IListener> listener, Ptr<IParser> parser, Ptr<IActor> actor);

    virtual void startListen() override;

    virtual Ptr <IParser> getParser() override;
    virtual Ptr <IActor> getActor()  override;
    virtual Ptr <IListener> getListener() override;

private:
    // Callbacks for IListener
    void onListenerNewMsg(std::string data);
    void onListenerError(std::string&& error);


    // Callback for IParser
    void onParserSuccess(SensorInfo sensorInfo);
    void onParserError(std::string&& error);

};

#endif //QUERYANALYZINGPACKET_IQUERYHANDLER_H
