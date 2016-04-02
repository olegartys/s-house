//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_UTILS_H
#define QUERYANALYZINGPACKET_UTILS_H

#include <string>
#include <sstream>
#include "SensorInfo.h"

std::string to_string(const SensorName& sensorName);
std::ostream& operator<< (std::ostream& str, const SensorName& sensorName);

std::string to_string(const SensorInfo& sensorInfo);
std::ostream& operator<< (std::ostream& str, const SensorInfo& i);


#endif //QUERYANALYZINGPACKET_UTILS_H
