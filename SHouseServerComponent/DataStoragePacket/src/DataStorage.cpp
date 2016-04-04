//
// Created by eninng on 29.03.16.
//

#include "DataStorage.h"
#include <iostream>

IDataStorage::ReturnCode   DataStorage::createConnectionWithSqlDB(std::string userName, std::string password, std::string DBname) {
    config->user = userName;
    config->password = password;
    config->database = DBname;
    config->debug = true;
    db = std::shared_ptr<sqlpp::mysql::connection>(new sqlpp::mysql::connection(config));
    return ReturnCode::SUCCESS ;
}

IDataStorage::ReturnCode DataStorage::setStateByUserQuery(std::string userSensorName, std::string newState, OnStateChangedCallbackType onStateChangedCallbackType,
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

    std::string systemSensorName, sensorType;
    auto getSensTypeAndSysSensNameReturn = getSensorTypeAndSystemSensorName(userSensorName, systemSensorName, sensorType);

    if (getSensTypeAndSysSensNameReturn == ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB) {
        onErrorCallbackType("NO SUCH USER SENSOR NAME IN DATABASE");
        return ReturnCode::SUCCESS;
    } else if (getSensTypeAndSysSensNameReturn == ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB) {
        onErrorCallbackType("THERE ARE TOO MANY USER SENSOR NAMES IN DATABASE");
        return ReturnCode::SUCCESS;
    } else if (getSensTypeAndSysSensNameReturn == ErrorCode::SUCCESS) {
        // ДАЛЕЕ 3 ИСХОДА! 1 - ОШИБКА, 2 - СОСТОЯНИЕ ТАКОЕ ЖЕ, 3 - СОСТОЯНИЕ ОТЛИЧАЕТСЯ.
        std::string currentState;
        if (sensorType == sensorTypeConstants.monitorType) {
            onErrorCallbackType("CAN'T CHANGE MONITOR TYPE");
            return ReturnCode::SUCCESS;
        }
        auto getDataReturn = getData(systemSensorName, sensorType, currentState);
        if (getDataReturn == ErrorCode::SUCCESS) {
            if (currentState == newState) {
//                CОСТОЯНИЕ ТАКОЕ ЖЕ
                onOldStateCallbackType();
                return ReturnCode::SUCCESS;
            } else {
//                СОСТОЯНИЕ ИЗМЕНИЛОСЬ
                onStateChangedCallbackType();
                return ReturnCode::SUCCESS;
            }
        } else if (getDataReturn == ErrorCode::THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB) {
            onErrorCallbackType("THERE ARE TOO MANY SYSTEM SENSOR NAMES IN DB");
            return ReturnCode::SUCCESS;
        } else if (getDataReturn == ErrorCode::THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAMES_IN_DB) {
            onErrorCallbackType("THERE ARE NOT SHUCH SYSTEM SENSOR NAMES IN DB");
            return ReturnCode::SUCCESS;
        }
    }
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::getState(std::string userSensorName, OnSuccessCallbackType, OnErrorCallbackType)
{
    std::string systemSensorName, sensorType;
    auto funcReturn = getSensorTypeAndSystemSensorName(userSensorName, systemSensorName, sensorType);
    if (funcReturn == ErrorCode::SUCCESS) {

        std::string state;
        auto getDataReturn = getData(systemSensorName, sensorType, state);
        if (getDataReturn == ErrorCode::SUCCESS) {
            // При удаче в колбэк передается строка с состоянием
            /*
             * OnSuccessCallbackType
             */
        } else if (true) {

        }

    }
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::setStateByClientQuery(std::string systemSensorName, std::string sensorType, OnSuccessCallbackType,
                                       OnErrorCallbackType) {
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::addSensor() {
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::removeSensor() {
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::getSystemSensorNameByUserSensorName(std::string userSensorName, std::string& systemSensorName)
{

    int inc = 0;
    const auto tabMainTable  = DataDB::MainTable();
    std::string sSName;
    for (const auto& row : db->operator()(sqlpp::select(tabMainTable.systemSensorName).from(tabMainTable).
            where(tabMainTable.userSensorName == userSensorName))) {
        inc++;
        sSName  = row.systemSensorName;
    }
    if (inc == 1) {
        systemSensorName = sSName;
        return ReturnCode::SUCCESS;
    } else if (inc == 0 ) {
        return ReturnCode::NO_SUCH_USER_SENSOR_NAME_IN_DB;
    } else if (inc > 1) {
        return ReturnCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB;
    }
}



/*
 *
 * ДАЛЬШЕ ПРИВАТНЫЕ МЕТОДЫ!
 *
 *
 */

IDataStorage::ErrorCode DataStorage::getSensorTypeAndSystemSensorName(std::string userSensorName, std::string &systemSensorName,
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
        return ErrorCode::SUCCESS;
    } else if (inc == 0 ) {
        return ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB;
    } else if (inc > 1) {
        return ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB;
    }
}


IDataStorage::ErrorCode DataStorage::getData(std::string systemSensorName, std::string sensorType, std::string& currentState)
{

    int inc = 0;
    std::string data;
    if (sensorType == sensorTypeConstants.binaryType) {
        auto tabBinaryType = DataDB::BinaryType();
        int inc = 0;
        for (const auto& row : db->operator()(sqlpp::select(tabBinaryType.state).from(tabBinaryType).
                                             where(tabBinaryType.systemSensorName == systemSensorName))) {
            data = row.state;
            ++inc;
        }
        if (inc == 0) {
            return ErrorCode::THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAMES_IN_DB;
        } else if (inc > 1) {
            return ErrorCode::THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB;
        } else if (inc == 1) {
            currentState = data;
            return ErrorCode::SUCCESS;
        }
    } else if (sensorType == sensorTypeConstants.manyStatesType) {
        auto tabManyStatesType = DataDB::ManyStatesType();
        int inc = 0;
        for (const auto& row : db->operator()(sqlpp::select(tabManyStatesType.state).from(tabManyStatesType).
                where(tabManyStatesType.systemSensorName == systemSensorName))) {
            data = row.state;
            ++inc;
        }
        if (inc == 0) {
            return ErrorCode::THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAMES_IN_DB;
        } else if (inc > 1) {
            return ErrorCode::THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB;
        } else if (inc == 1) {
            currentState = data;
            return ErrorCode::SUCCESS;
        }
    }
    return ErrorCode::SUCCESS;
}






// TODO придумать, как проверить установилось ли соеденение.