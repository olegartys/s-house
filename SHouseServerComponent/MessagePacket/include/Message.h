//
// Created by olegartys on 31.03.16.
//

#ifndef QUERYANALYZINGPACKET_MESSAGE_H
#define QUERYANALYZINGPACKET_MESSAGE_H


#include <stddef.h>
#include <string>
#include <json/json.h>

#include "ISerializable.h"

/**
 * This class describes a Message that would be transfered between different components of a system.
 */
struct Message: public ISerializable {

    /**
     * This enumeration describes possible query types that can be treated.
     * @param GET_STATE Means to transfer a state (data) field of sensor object.
     * @param SET_STATE This query means to set sensor into the state given in the field data.
     * @param DEPRECATED_QUERY Is the state that would be treated as mistake.
     */
    struct QueryType {
        enum { GET_STATE, SET_STATE, DEPRECATED_QUERY };
    };

    /**
    * This enumeration describes possible sensor types that can be treated.
    * @param MONITORING Is the sensor that can't have state.
    * @param MULTIPLE Is the sensor that could have a range of states (i.e. thermometer).
    * @param BINARY Is the sensor that could have only two states: 0 or 1 (i.e light controller).
    * @param DEPRECATED_QUERY Is the type that would be treated as mistake.
    */
    struct SensorType {
        enum { MONITORING, MULTIPLE, BINARY, DEPRECATED_TYPE };
    };

    /**
     * @brief Constructs the message object.
     * @param src String representation of the message that would be deserialized into the object.
     * @param res Is a return code of construction operation (to make exception-safe entity).
     */
    Message(const std::string& src, ReturnCode& res);

    virtual std::string serialize(bool styled= false) const override;

    virtual ReturnCode deserialize(const std::string &src) override;

    /**
     * An id of message.
     */
    int id;

    /**
     * A type of the message.
     */
    int queryType;

    /**
     * A type of the sensor.
     */
    int sensorType;

    /**
     * The sensor name.
     */
    std::string sensorName;

    /**
     * The data which can be used (and it is expected so) for transfering state.
     */
    std::string data;

};

//    int getID()                 const { return id; }
//    int getQueryType()          const { return queryType; }
//    int getSensorType()         const { return sensorType; }
//    std::string getSensorName() const { return sensorName; }
//    std::string getData()       const { return data; }


#endif //QUERYANALYZINGPACKET_MESSAGE_H
