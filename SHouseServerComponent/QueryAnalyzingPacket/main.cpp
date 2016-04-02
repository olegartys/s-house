#include <iostream>
#include <json/json.h>
#include "QueryHandler.h"
#include "QueryListener.h"
#include "QueryParser.h"

using namespace std;

int main() {
    //QueryHandler handler(std::shared_ptr<IListener>(new QueryListener()), std::shared_ptr<IParser>(new QueryParser()), std::shared_ptr<IActor>(new IActor()));
    //handler.startListen();

    Json::Value root;
    root["encoding"] = "UTF-8";
    root["indent"] = "kek";
    std::cout << root;
    return 0;
}