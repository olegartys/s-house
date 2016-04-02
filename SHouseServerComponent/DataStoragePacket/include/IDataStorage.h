//
// Created by eninng on 26.03.16.
//

#ifndef SHOUSEDATASTORAGE_IDATASTORAGE_H
#define SHOUSEDATASTORAGE_IDATASTORAGE_H

#include <string>
#include <memory>

#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>



class IDataStorage {
public:

    struct ReturnCode {
        enum returnCode : int { SUCCESS = 0,
                                THERE_ARE_NULLPTR_CALLBACKS,


        };
    };

    struct ErrorCode  {
        enum errorCode : int {  NO_SUCH_USER_SENSOR_NAME_IN_DB=1,
                                THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB,
                                CANNOT_CHANGE_MONITOR_TYPE_STATE
        };
    };

//    struct SensorType {
//        enum sensorType : std::string {
//            BINARY="BINARY",
//            manyStatesType = "MANY_STATES_TYPE",
//            monitorType = "MONITOR_TYPE"
//
//        };
//    };
    struct SensorType {
        const std::string binaryType =  "BINARY_TYPE";
        const std::string manyStatesType = "MANY_STATES_TYPE";
        const std::string monitorType = "MONITOR_TYPE";
    };

    using OnStateChangedCallbackType = std::function<int()>;
    using OnOldStateCallbackType = std::function<int(std::string stateIsOld)>;

    using OnSuccessCallbackType = std::function<int()>;
    using OnErrorCallbackType = std::function<int(std::string errorMsg)>;

    virtual ~IDataStorage() = default;

    /*
     * @brief Creating connection with database. It's first needed function.
     */
    virtual int createConnectionWithSqlDB(std::string userName, std::string password, std::string DBname) =0;

    /*
     * @brief Change sensor's state by User query.
     */
    virtual int setStateByUserQuery(std::string userSensorName, std::string newState, OnStateChangedCallbackType,
                                                                                      OnOldStateCallbackType ,
                                                                                      OnErrorCallbackType) =0;
    /*
     * @brief Get state,using UserSensorName.
     */
    virtual int getState(std::string userSensorName, OnSuccessCallbackType, OnErrorCallbackType) =0;

    /*
     * @brief Change data on DB by Client query. (Client = FA).
     */
    virtual int setStateByClientQuery(std::string systemSensorName, std::string sensorType,
                                                                    OnSuccessCallbackType,
                                                                    OnErrorCallbackType) =0;

    /*
    * @param std::string userSensorName - Sensor Name by User.
    *        std::string& linkOnSystemSensorName - link on System Sensor Name in which the recorded value.
    */
    /*
     * @brief Get System Sensor Name into link and return 0 at success or ErrorCode.
     */
    virtual int getSystemSensorNameByUserSensorName(std::string userSensorName, std::string& linkOnUserSystemName) = 0;

    virtual int addSensor() =0;

    virtual int removeSensor() =0;

protected:

    //virtual int getData(std::string systemSensorName, std::string& ) =0;

    const std::shared_ptr<sqlpp::mysql::connection_config> config = std::make_shared<sqlpp::mysql::connection_config>();

    std::shared_ptr<sqlpp::mysql::connection> db;
    SensorType sensorTypeConstants;
    //ErrorCode errorCode;

};


#endif //SHOUSEDATASTORAGE_IDATASTORAGE_H

// TODO Изменить прототип onSuccess!