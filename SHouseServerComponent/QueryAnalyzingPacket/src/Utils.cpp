//
// Created by olegartys on 27.03.16.
//

#include "Utils.h"

std::string to_string(const SensorName& sensorName) {
    std::stringstream ss;
    ss << sensorName.sensorUserName << "," << sensorName.sensorSystemName;
    return ss.str();
}


std::string to_string(const SensorInfo& sensorInfo) {
    std::stringstream ss;
    ss << to_string(sensorInfo.sensorName) << "," << sensorInfo.queryType << "," << sensorInfo.data;
    return ss.str();
}

std::ostream& operator<< (std::ostream& str, const SensorName& sensorName) {
    str << to_string(sensorName);
    return str;
}


std::ostream& operator<< (std::ostream& str, const SensorInfo& sensorInfo) {
    str << to_string(sensorInfo);
    return str;
}