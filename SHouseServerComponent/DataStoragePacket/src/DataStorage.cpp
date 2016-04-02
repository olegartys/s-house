//
// Created by eninng on 29.03.16.
//

#include "DataStorage.h"
#include <iostream>

int DataStorage::createConnectionWithSqlDB(std::string userName, std::string password, std::string DBname) {
    config->user = userName;
    config->password = password;
    config->database = DBname;
    config->debug = true;
    db = std::shared_ptr<sqlpp::mysql::connection>(new sqlpp::mysql::connection(config));
    return 0;
}

int DataStorage::getSystemSensorNameByUserSensorName(std::string userSensorName, std::string& systemSensorName)
{

    int inc = 0;
    const auto tabMainTable  = DataDB::MainTable();
    std::string sSName;
    for (const auto& row : db->operator()(sqlpp::select(tabMainTable.systemSensorName).from(tabMainTable).where(tabMainTable.userSensorName == userSensorName))) {
        inc++;
        sSName  = row.systemSensorName;
    }
    if (inc == 1) {
        systemSensorName = sSName;
        return 0;
    } else if (inc == 0 ) {
        return ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB;
    } else if (inc > 1) {
        return ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB;
    }
}

int DataStorage::setStateByUserQuery(std::string userSensorName, std::string newState, OnStateChangedCallbackType onStateChangedCallbackType,
                                     OnOldStateCallbackType onOldStateCallbackType, OnErrorCallbackType onErrorCallbackType)
{
    /*
     * @brief Check that callbacks were transferred.
     */

    if (onStateChangedCallbackType == nullptr || onOldStateCallbackType == nullptr || onErrorCallbackType == nullptr) {
        return ReturnCode::THERE_ARE_NULLPTR_CALLBACKS;
    }
    /*
     * @brief Create variable of MainTable.
     */

    const auto  tabMainTable = DataDB::MainTable();

    int getSensTypeAndSysSensNameReturn;
    std::string systemSensorName, sensorType;
    getSensTypeAndSysSensNameReturn = getSensorTypeAndSystemSensorName(userSensorName, systemSensorName, sensorType);

    if (getSensTypeAndSysSensNameReturn == ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB) {
        onErrorCallbackType("NO SUCH USER SENSOR NAME IN DATABASE");
        return ReturnCode::SUCCESS;
    } else if (getSensTypeAndSysSensNameReturn == ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB) {
        onErrorCallbackType("THERE ARE TOO MANY USER SENSOR NAMES IN DATABASE");
        return ReturnCode::SUCCESS;
    } else if (getSensTypeAndSysSensNameReturn == 0) {
        // ДАЛЕЕ 3 ИСХОДА! 1 - ОШИБКА, 2 - СОСТОЯНИЕ ТАКОЕ ЖЕ, 3 - СОСТОЯНИЕ ОТЛИЧАЕТСЯ.
        std::string currentState;
        int getDataReturn = -1;
        if (sensorType == sensorTypeConstants.monitorType) {
            onErrorCallbackType("CAN'T CHANGE MONITOR TYPE CALLBACK");
            return ReturnCode::SUCCESS;
        }
        getDataReturn = getData(systemSensorName, sensorType, currentState);
    }
    return 0;
}

int DataStorage::getState(std::string userSensorName, OnSuccessCallbackType, OnErrorCallbackType)
{
    return 0;
}

int DataStorage::setStateByClientQuery(std::string systemSensorName, std::string sensorType, OnSuccessCallbackType,
                                       OnErrorCallbackType) {
    return 0;
}

int DataStorage::addSensor() {
    return 0;
}

int DataStorage::removeSensor() {
    return 0;
}

int DataStorage::getData(std::string systemSensorName, std::string sensorType, std::string& currentState)
{

    int inc = 0;
    std::string data;
        if (sensorType == sensorTypeConstants.binaryType) {
            auto tabBinaryType = DataDB::BinaryType();
            for (const auto& row : db->operator()(sqlpp::select(tabBinaryType.state).from(tabBinaryType).where(tabBinaryType.systemSensorName == systemSensorName))) {

            }

        }

    return 0;
}

int DataStorage::getSensorTypeAndSystemSensorName(std::string userSensorName, std::string &systemSensorName,
                                                  std::string &sensorType) {
    int inc = 0;
    const auto tabMainTable  = DataDB::MainTable();
    std::string sSName, sensType;
    for (const auto& row : db->operator()(sqlpp::select(tabMainTable.systemSensorName, tabMainTable.sensorType)
                                                  .from(tabMainTable).where(tabMainTable.userSensorName == userSensorName))) {
        inc++;
        sensType = row.sensorType;
        sSName  = row.systemSensorName;
    }
    if (inc == 1) {
        sensorType = sensType;
        systemSensorName = sSName;
        return 0;
    } else if (inc == 0 ) {
        return ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB;
    } else if (inc > 1) {
        return ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB;
    }
}






// TODO придумать, как проверить установилось ли соеденение.