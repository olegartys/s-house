//
// Created by eninng on 26.03.16.
//

#ifndef SHOUSEDATASTORAGE_IDATASTORAGE_H
#define SHOUSEDATASTORAGE_IDATASTORAGE_H

#include <string>
#include <memory>

#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>

/*
 * @brief Class IDataStorage is an interface between user or client and Sql DataBase.
 * It's pure virtual class.
 * This class provide minimum of necessary interface.
 * This class depends on library SQLPP11, which in turn depends on the library MYSQL-CLIENT.
 * All public methods of this class return enum class ReturnCode,which is described in this class.
 * All public methods has as parameters CallBacks, which are used in new thread on Success or on Error.
 */

class IDataStorage {
public:

    /*
     * @brief Class ReturnCode is the return value for public methods.
     */
    enum class ReturnCode { SUCCESS=0,
                            THERE_ARE_NULLPTR_CALLBACKS,
                            THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB,
                            NO_SUCH_USER_SENSOR_NAME_IN_DB,
                            WRONG_CONFIG,
                            EXCEPTION_ERROR,
                            THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB,
                            THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAME_IN_DB,
                            WRONG_SENSOR_TYPE
    };
    /*
     * @brief Struct SensorType has 3 std::string constants with types of Sensors: BINARY_TYPE, MANY_STATES_TYPE, MONITOR_TYPE.
     */
    struct SensorType {
        const std::string binaryType =  "BinaryType";
        const std::string manyStatesType = "ManyStatesType";
        const std::string monitorType = "MonitorType";
    };
    /*
     * @brief Struct BinaryTypeStates has 2 std::string constants with states for Binary Type Sensor: ON, OFF.
     */
    struct BinaryTypeStates {
        const std::string on = "ON";
        const std::string off = "OFF";
    };

    /*
     * @brief Usings for callbacks.
     */
    using OnStateChangedCallbackType = std::function<int()>;
    using OnOldStateCallbackType = std::function<int()>;
// QUERY ID??????
    using OnSuccessCallbackType = std::function<int(std::string data)>;
    using OnErrorCallbackType = std::function<int(std::string errorMsg)>;

    virtual ~IDataStorage() = default;

    /*
     * @brief Method createConnectionWithSqlDb create connection with database. It's first needed function.
     * It return ReturnCode and has next parameters:
     * @param[in] userName - std::string with name of mysql User.
     * @param[in] password - std::string with user's mysql password.
     * @param[in] DBname - std::string with name of using DataBase.
     * @return ReturnCode - Enum class ReturnCode.
     * It return ReturnCode::Success if config is right or return WRONG_CONFIG if config iw wrong.
     */
    virtual ReturnCode createConnectionWithSqlDB(std::string userName, std::string password, std::string DBname) =0;

    /*
     *  @brief Method setStateByUserQuery change sensor's state by User query.
     *  It return ReturnCode and has next parameters:
     *  @param[in] userSensorName - std::string with User Sensor Name.
     *  @param[in] newState - std::string with new State which user choose.
     *  @param[in] OnStateChangedCallbackType - callback which is used when state of sensor changed.
     *  @param[in] OnOldStateCallbackType - callback which is used when state of sensor changed.
     *  @param[in] OnErrorCallbackType - callback which is used when any error happened.
     *
     *  It return ReturnCode::THERE_ARE_NULL_PTR_CALLBACKS if there 1 or more nullptr callback.
     *  In other way it return ReturnCode::SUCCESS.
     */
    virtual ReturnCode setStateByUserQuery(std::string userSensorName, std::string newState, OnStateChangedCallbackType,
                                                                                      OnOldStateCallbackType ,
                                                                                      OnErrorCallbackType) =0;
    /*
     * @brief Method getState return to User current state of sensor.
     * It return ReturnCode and has next parameters:
     * @param[in] userSensorName - std::string with User Sensor Name.
     * @param[in] OnSuccessCallbackType - callback which is used if state is received without errors.
     * @param[in] OnErrorCallBackType - callback which is used when any error happened.
     */
    virtual ReturnCode getState(std::string userSensorName, OnSuccessCallbackType, OnErrorCallbackType) =0;

    /*
     * @brief Method setStateByClientQuery change data in DB by Client query. (Client = FA).
     * It return ReturnCode and has next parameters:
     * @param[in] systemSensorName - std::string with System Sensor Name.
     * @param[in] sensorType - std::string with type of sensor.
     * @param[in] newState - std::string with new state.
     * @param[in] OnSuccessCallbackType - callback which is used if state is changed without errors.
     * @param[in] OnErrorCallBackType - callback which is used when any error happened.
     */
    virtual ReturnCode setStateByClientQuery(std::string systemSensorName, std::string sensorType, std::string newState,
                                                                    OnSuccessCallbackType,
                                                                    OnErrorCallbackType) =0;

    /*
     * @brief Method getSystemSensorNameByUserSensorName - write systemSensorName into reference on std::string.
     * It return ReturnCode and has next parameters:
     * @param[in]  userSensorName - std::string with User Sensor Name, which is used for finding System Sensor Name in DB.
     * @param[in]  systemSensorName - std::string& - reference on system sensor name in which will be recorded value.
     * It return ReturnCode::NO_SUCH_USER_SENSOR_NAME if there are not such User sensor names in db.
     * It return ReturnCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB if there are more than 1 such User sensor name.
     * In other way it return ReturnCode::SUCCESS.
     */

    virtual ReturnCode getSystemSensorNameByUserSensorName(std::string userSensorName, std::string& systemSensorName) = 0;

    virtual ReturnCode addSensor() =0;

    virtual ReturnCode removeSensor() =0;
    /*
     * Данный тип принимает UserSensorName, т.к. в таком случае ему не нужно узнавать тип датчика.
     */
    virtual ReturnCode  getFAid(std::string userSensorName, std::string& FAid) =0;

protected:

    /*
     * @brief Class ErrorCode is the return value for private and protected methods.
     */
    enum class ErrorCode {  SUCCESS=0,
        NO_SUCH_USER_SENSOR_NAME_IN_DB,
        THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB,
        THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB,
        CANNOT_CHANGE_MONITOR_TYPE_STATE,
        THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAMES_IN_DB,
        EXCEPTION_ERROR,
        WRONG_SENSOR_TYPE
    };

    //virtual int getData(std::string systemSensorName, std::string& ) =0;
    /*
     * @brief Shared_ptr on config.
     */
    const std::shared_ptr<sqlpp::mysql::connection_config> config = std::make_shared<sqlpp::mysql::connection_config>();
    /*
     * @brief Shared_ptr on DB.
     */
    std::shared_ptr<sqlpp::mysql::connection> db;
    SensorType sensorTypeConstants;

};

#endif //SHOUSEDATASTORAGE_IDATASTORAGE_H
