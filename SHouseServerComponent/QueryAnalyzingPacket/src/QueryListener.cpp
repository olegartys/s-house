//
// Created by olegartys on 27.03.16.
//

#include "QueryListener.h"

int QueryListener::listen(IListener::OnNewQueryCatchedCallbackType onNew,
                          IListener::OnErrorCatchedCallbackType onError, const IHandler* handler) {

    if (handler != nullptr) {
        this->handler = handler;
    }

    onNew("onNewTest");
    onError("onErrorTest");

    return 0;
}

const IHandler* QueryListener::getHandler() {
    return handler;
}
