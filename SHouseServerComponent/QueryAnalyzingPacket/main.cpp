#include <iostream>
#include "QueryHandler.h"
#include "QueryListener.h"
#include "QueryParser.h"

using namespace std;

int main() {
    QueryHandler handler(std::shared_ptr<IListener>(new QueryListener()), std::shared_ptr<IParser>(new QueryParser()), std::shared_ptr<IActor>(new IActor()));
    handler.startListen();
    return 0;
}