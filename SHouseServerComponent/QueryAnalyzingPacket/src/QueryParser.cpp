//
// Created by olegartys on 27.03.16.
//

#include "QueryParser.h"

int QueryParser::parse(const std::string& data, IParser::OnSuccess onSuccess, IParser::OnError onError) {
    SensorInfo i;
    i.data = "test";
    i.queryType = "test";
    SensorName n;
    n.sensorSystemName = "test";
    n.sensorUserName = "test";
    i.sensorName = n;
    onSuccess(i);
    onError("parseErrorTest");
    return 0;
}
