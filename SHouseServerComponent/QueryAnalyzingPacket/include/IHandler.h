//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_IHANDLER_H
#define QUERYANALYZINGPACKET_IHANDLER_H

#include <memory>
#include "IActor.h"
#include "IParser.h"
#include "IListener.h"

class IListener;

/**
 * @brief This interface represents Handler objects.
 * Handler is the object who owns all the workers - listener, parser and actor - and who triggers all their interfaces.
 * From handler all callbacks for the workers should be set.
 */
class IHandler {
public:
    template <typename T> using Ptr = std::shared_ptr<T>; // to make code shorter

    virtual ~IHandler() = default;

    /**
     * @brief Here we start listening and delegate work with recieved data to workers.
     */
    virtual void startListen() = 0;

    /**
     * @brief This method should return listener object.
     */
    virtual Ptr<IListener> getListener() = 0;

    /**
     * @brief This method should return parser object.
     */
    virtual Ptr<IParser> getParser() = 0;

    /**
     * @brief This method should return actor object.
     */
    virtual Ptr<IActor> getActor() = 0;

protected:
    // Workers
    Ptr<IListener> listener;
    Ptr<IParser> parser;
    Ptr<IActor> actor;

};

#endif //QUERYANALYZINGPACKET_IHANDLER_H
