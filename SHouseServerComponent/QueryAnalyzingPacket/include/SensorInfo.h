//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_SENSORTINFO_H
#define QUERYANALYZINGPACKET_SENSORTINFO_H

#include <string>
#include <sstream>
#include "SensorName.h"

struct SensorInfo final {
    SensorName sensorName;
    std::string queryType;
    std::string data;
};


#endif //QUERYANALYZINGPACKET_SENSORTINFO_H
