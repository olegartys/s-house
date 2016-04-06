//
// Created by eninng on 29.03.16.
//

#include "DataStorage.h"
#include <iostream>


IDataStorage::ReturnCode   DataStorage::createConnectionWithSqlDB(std::string userName, std::string password, std::string DBname) {
// Use standard values for this project.
    try {
        config->user = userName;
        config->password = password;
        config->database = DBname;
        config->debug = true;
//    set config into connection.
        db = std::shared_ptr<sqlpp::mysql::connection>(new sqlpp::mysql::connection(config));
        return ReturnCode::SUCCESS;
    } catch (std::exception& e) {
        return ReturnCode::WRONG_CONFIG;
    }
}

IDataStorage::ReturnCode DataStorage::setStateByUserQuery(std::string userSensorName, std::string newState, OnStateChangedCallbackType onStateChangedCallback,
                                     OnOldStateCallbackType onOldStateCallback, OnErrorCallbackType onErrorCallback)
{

//    Check that callbacks were transferred.


    if (onStateChangedCallback== nullptr || onOldStateCallback == nullptr || onErrorCallback == nullptr) {

                return ReturnCode::THERE_ARE_NULLPTR_CALLBACKS;
    }

//     Create variable of MainTable.
    const auto  tabMainTable = DataDB::MainTable();

    std::string systemSensorName, sensorType;
//    get System Name and Type of sensor.
    auto getSensTypeAndSysSensNameReturn = getSensorTypeAndSystemSensorName(userSensorName, systemSensorName, sensorType);

//    Error processing
    if (getSensTypeAndSysSensNameReturn == ErrorCode::EXCEPTION_ERROR) {
        onErrorCallback("EXEPTION");
        return ReturnCode::SUCCESS;
    }

    if (getSensTypeAndSysSensNameReturn == ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB) {
        onErrorCallback("NO SUCH USER SENSOR NAME IN DATABASE");
        return ReturnCode::SUCCESS;
    }
    if (getSensTypeAndSysSensNameReturn == ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB) {
        onErrorCallback("THERE ARE TOO MANY USER SENSOR NAMES IN DATABASE");
        return ReturnCode::SUCCESS;
    }
    //  SUCCESS processing
    if (getSensTypeAndSysSensNameReturn == ErrorCode::SUCCESS) {
        // ДАЛЕЕ 3 ИСХОДА! 1 - ОШИБКА, 2 - СОСТОЯНИЕ ТАКОЕ ЖЕ, 3 - СОСТОЯНИЕ ОТЛИЧАЕТСЯ.
        std::string currentState;
        if (sensorType == sensorTypeConstants.monitorType) {
            onErrorCallback("CAN'T CHANGE MONITOR TYPE");
            return ReturnCode::SUCCESS;
        }
//      Get current sensor's state
        auto getDataReturn = getData(systemSensorName, sensorType, currentState);
//      IF SUCCESS
        if (getDataReturn == ErrorCode::SUCCESS) {
            if (currentState == newState) {
//                CОСТОЯНИЕ ТАКОЕ ЖЕ
                onOldStateCallback();
                return ReturnCode::SUCCESS;
            } else {
//                СОСТОЯНИЕ ИЗМЕНИЛОСЬ
                onStateChangedCallback();
                return ReturnCode::SUCCESS;
            }
//       IF ERROR
        } else if (getDataReturn == ErrorCode::THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB) {
            onErrorCallback("THERE ARE TOO MANY SYSTEM SENSOR NAMES IN DB");
            return ReturnCode::SUCCESS;
        } else if (getDataReturn == ErrorCode::THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAMES_IN_DB) {
            onErrorCallback("THERE ARE NOT SUCH SYSTEM SENSOR NAMES IN DB");
            return ReturnCode::SUCCESS;
        } else if (getDataReturn == ErrorCode::EXCEPTION_ERROR) {
            onErrorCallback("Error");
            return ReturnCode::SUCCESS;
        }
    }
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::getState(std::string userSensorName, OnSuccessCallbackType onSuccessCallback,
                                               OnErrorCallbackType onErrorCallback)
{
    if (onErrorCallback == nullptr ||  onSuccessCallback == nullptr) {
        return ReturnCode::THERE_ARE_NULLPTR_CALLBACKS;
    }
    // get data
    std::string systemSensorName, sensorType;
    auto funcReturn = getSensorTypeAndSystemSensorName(userSensorName, systemSensorName, sensorType);
    if (funcReturn == ErrorCode::SUCCESS) {
        std::string state;
        auto getDataReturn = getData(systemSensorName, sensorType, state);
//      IF SUCCESS
        if (getDataReturn == ErrorCode::SUCCESS) {
            onSuccessCallback(state);
            return ReturnCode::SUCCESS;
        }
//      IF ERROR
        if (getDataReturn == ErrorCode::EXCEPTION_ERROR) {
            onErrorCallback("Exception");
            return ReturnCode::SUCCESS;
        }
        if (getDataReturn == ErrorCode::THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAMES_IN_DB) {
            onErrorCallback("THERE ARE NOT SUCH SYSTEM SENSOR NAME IN DB");
            return ReturnCode::SUCCESS;
        }
        if (getDataReturn == ErrorCode::THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB) {
            onErrorCallback("THERE ARE TOO MANY SYSTEM SENSOR NAME IN DB");
            return ReturnCode::SUCCESS;
        }
    }
    if (funcReturn == ErrorCode::EXCEPTION_ERROR) {
        onErrorCallback("Exception");
        return ReturnCode::SUCCESS;
    }
    if (funcReturn == ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB) {
        onErrorCallback("NO SUCH USER SENSOR NAME IN DB");
        return ReturnCode::SUCCESS;
    }
    if (funcReturn == ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB) {
        onErrorCallback("THERE ARE TOO MANY USER SENSOR NAMES IN DB");
        return ReturnCode::SUCCESS;
    }
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::setStateByClientQuery(std::string systemSensorName, std::string sensorType, std::string newState,
                                                            OnSuccessCallbackType onSuccessCallback,
                                                            OnErrorCallbackType onErrorCallback) {
    if (onSuccessCallback == nullptr || onErrorCallback == nullptr) {
        return ReturnCode::THERE_ARE_NULLPTR_CALLBACKS;
    }

//    std::string exec;
//            db->execute("update BinaryType set state='on' where systemSensorName='light1'");
//
//    exec = "update " + sensorType + " set state='" + newState + "' where systemSensorName='" + systemSensorName + "'";
//    db->execute(exec);

    try {
        if (sensorType == sensorTypeConstants.manyStatesType) {
            auto tabManyStatesType = DataDB::ManyStatesType();
            db->operator()(update(tabManyStatesType).set(tabManyStatesType.state = newState)
                                   .where(tabManyStatesType.systemSensorName == systemSensorName));
            onSuccessCallback(nullptr);
            return ReturnCode::SUCCESS;
        }

//        db->execute("UPDATE BinaryType SET state='off' WHERE (BinaryType.systemSensorName='light1')");

        if (sensorType == sensorTypeConstants.binaryType) {
            auto tabBinaryType = DataDB::BinaryType();
//            db->update()
            db->operator()(sqlpp::update(tabBinaryType).set(tabBinaryType.state = "ON")
                                   .where(tabBinaryType.systemSensorName =="light1"));
            onSuccessCallback(nullptr);
            return ReturnCode::SUCCESS;
        }

        if (sensorType == sensorTypeConstants.monitorType) {
            auto tabMonitorType = DataDB::MonitorType();
            db->operator()(update(tabMonitorType).set(tabMonitorType.state = newState)
                                   .where(tabMonitorType.systemSensorName == systemSensorName));
            onSuccessCallback(nullptr);
            return ReturnCode::SUCCESS;
        }
        // Дальше дергается колбэк ошибки
    } catch(std::exception& e) {
        onErrorCallback(e.what());
    }
    onErrorCallback("error");
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
        ++inc;
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
    try {
        int inc = 0;
        const auto tabMainTable = DataDB::MainTable();
        std::string sSName, sensType;
        for (const auto &row : db->operator()(sqlpp::select(tabMainTable.systemSensorName, tabMainTable.sensorType)
                                                      .from(tabMainTable).where(
                tabMainTable.userSensorName == userSensorName))) {
            inc++;
            sensType = row.sensorType;
            sSName = row.systemSensorName;
        }
        if (inc == 1) {
            sensorType = sensType;
            systemSensorName = sSName;
            return ErrorCode::SUCCESS;
        } else if (inc == 0) {
            return ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB;
        } else if (inc > 1) {
            return ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB;
        }
    } catch (std::exception& e) {
        return ErrorCode::EXCEPTION_ERROR;
    }
}


IDataStorage::ErrorCode DataStorage::getData(std::string systemSensorName, std::string sensorType, std::string& currentState) {

    int inc = 0;
    std::string data;
    try {
        if (sensorType == sensorTypeConstants.binaryType) {
            auto tabBinaryType = DataDB::BinaryType();
            int inc = 0;
            for (const auto &row : db->operator()(sqlpp::select(tabBinaryType.state).from(tabBinaryType).
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
            for (const auto &row : db->operator()(sqlpp::select(tabManyStatesType.state).from(tabManyStatesType).
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
        } else if (sensorType == sensorTypeConstants.monitorType) {
            auto tabMonitorType = DataDB::MonitorType();
            int inc = 0;
            for (const auto &row : db->operator()(sqlpp::select(tabMonitorType.state).from(tabMonitorType).
                    where(tabMonitorType.systemSensorName == systemSensorName))) {
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
    } catch (std::exception& ) {
        return ErrorCode::EXCEPTION_ERROR;
    }
    return ErrorCode::EXCEPTION_ERROR;
}




//TODO ПЕРЕДЕЛАТЬ setStateByClientQuery, т.к. update не работает. попробовать использовать execute.

// TODO придумать, как проверить установилось ли соеденение.