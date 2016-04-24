//
// Created by eninng on 29.03.16.
//

#include "DataStorage.h"
#include <iostream>


IDataStorage::ReturnCode DataStorage::connect(const std::string &userName, const std::string &password,
                                                const std::string &DBname)
{
// Use standard values for this project.
    try {
        config->user = userName;
        config->password = password;
        config->database = DBname;
        config->debug = true;
//    set config into connection.
        db = std::shared_ptr<sqlpp::mysql::connection>(new sqlpp::mysql::connection(config));
        isConnected = true;
        return ReturnCode::SUCCESS;
    } catch (std::exception& e) {
        isConnected = false;
        return ReturnCode::WRONG_CONFIG;
    }
}

IDataStorage::ReturnCode DataStorage::setState(const std::string &systemSensorName, const std::string &newState,
                                               OnSetStateSuccessCallbackType onSuccessCallback, OnErrorCallbackType onErrorCallback)
{
    if (isConnected == false) {
        return ReturnCode::WRONG_CONFIG;
    }

    if (onSuccessCallback == nullptr || onErrorCallback == nullptr) {

                return ReturnCode::NULLPTR_CALLBACKS;
    }
    int inc = 0;
    std::string sensorType;
    const auto tabMainTable = DataDB::MainTable();
    for (const auto &row : db->operator()(sqlpp::select(tabMainTable.sensorType)
                                                  .from(tabMainTable).where(
            tabMainTable.systemSensorName == systemSensorName))) {
        inc++;
        sensorType = row.sensorType;
    }
    if (inc == 0) {
        return ReturnCode::NO_SUCH_SS_NAME;
    } else if (inc > 1) {
        return ReturnCode::TOO_MANY_SS_NAMES;
    }
    std::string currentState;
    //      Get current sensor's state
    auto getDataReturn = getData(systemSensorName, sensorType, currentState);
//      IF SUCCESS
    if (getDataReturn == ErrorCode::SUCCESS) {
        if (currentState == newState) {
//                CОСТОЯНИЕ ТАКОЕ ЖЕ
            onSuccessCallback(false, currentState); // Передаю состояние
            return ReturnCode::SUCCESS;
        } else {
//                СОСТОЯНИЕ ИЗМЕНИЛОСЬ
            std::string exec;
            try {
                // использование execute безопасно так как мы уверены что тип и имя верные, иначе выполнение метода завершилось бы ранее
               // так же использование execute значительно сокращает количество кода, зависимого от конкретных таблиц.
                exec = "update " + sensorType + " set state='" + newState + "' where systemSensorName='" +
                                        systemSensorName + "'";
                db->execute(exec);
            } catch (std::exception& e) {
                onErrorCallback(e.what());
                return ReturnCode::EXCEPTION_ERROR;
            }
            onSuccessCallback(true, newState); // передаю новое состояние
//             ИЗМЕНЕНИЕ СОСТОЯНИЯ!!

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
    } else if (getDataReturn == ErrorCode::WRONG_SENSOR_TYPE) {
        onErrorCallback("Wrong Sensor Type");
        return ReturnCode::SUCCESS;
    }
    return ReturnCode::SUCCESS;
}


//IDataStorage::ReturnCode DataStorage::setStateByUserQuery(const std::string &userSensorName,
//                                                          const std::string &newState,
//                                                          OnStateChangedCallbackType onStateChangedCallback,
//                                                          OnOldStateCallbackType onOldStateCallback,
//                                                          OnErrorCallbackType onErrorCallback)
//{
//    if (isConnected == false) {
//        return ReturnCode::WRONG_CONFIG;
//    }
//
////    Check that callbacks were transferred.
//
//
//    if (onStateChangedCallback== nullptr || onOldStateCallback == nullptr || onErrorCallback == nullptr) {
//
//                return ReturnCode::NULLPTR_CALLBACKS;
//    }
//
////     Create variable of MainTable.
//    const auto  tabMainTable = DataDB::MainTable();
//
//    std::string systemSensorName, sensorType;
////    get System Name and Type of sensor.
//    auto getSensTypeAndSysSensNameReturn = getSensorTypeAndSystemSensorName(userSensorName, systemSensorName, sensorType);
//
////    Error processing
//    if (getSensTypeAndSysSensNameReturn == ErrorCode::EXCEPTION_ERROR) {
//        onErrorCallback("EXCEPTION");
//        return ReturnCode::SUCCESS;
//    }
//
//    if (getSensTypeAndSysSensNameReturn == ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB) {
//        onErrorCallback("NO SUCH USER SENSOR NAME IN DATABASE");
//        return ReturnCode::SUCCESS;
//    }
//    if (getSensTypeAndSysSensNameReturn == ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB) {
//        onErrorCallback("THERE ARE TOO MANY USER SENSOR NAMES IN DATABASE");
//        return ReturnCode::SUCCESS;
//    }
//    //  SUCCESS processing
//    if (getSensTypeAndSysSensNameReturn == ErrorCode::SUCCESS) {
//        // ДАЛЕЕ 3 ИСХОДА! 1 - ОШИБКА, 2 - СОСТОЯНИЕ ТАКОЕ ЖЕ, 3 - СОСТОЯНИЕ ОТЛИЧАЕТСЯ.
//        std::string currentState;
//        if (sensorType == sensorTypeConstants.monitorType) {
//            onErrorCallback("CAN'T CHANGE MONITOR TYPE");
//            return ReturnCode::SUCCESS;
//        }
////      Get current sensor's state
//        auto getDataReturn = getData(systemSensorName, sensorType, currentState);
////      IF SUCCESS
//        if (getDataReturn == ErrorCode::SUCCESS) {
//            if (currentState == newState) {
////                CОСТОЯНИЕ ТАКОЕ ЖЕ
//                onOldStateCallback();
//                return ReturnCode::SUCCESS;
//            } else {
////                СОСТОЯНИЕ ИЗМЕНИЛОСЬ
////             ИЗМЕНЕНИЕ СОСТОЯНИЯ!!
//                onStateChangedCallback();
//                return ReturnCode::SUCCESS;
//            }
////       IF ERROR
//        } else if (getDataReturn == ErrorCode::THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB) {
//            onErrorCallback("THERE ARE TOO MANY SYSTEM SENSOR NAMES IN DB");
//            return ReturnCode::SUCCESS;
//        } else if (getDataReturn == ErrorCode::THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAMES_IN_DB) {
//            onErrorCallback("THERE ARE NOT SUCH SYSTEM SENSOR NAMES IN DB");
//            return ReturnCode::SUCCESS;
//        } else if (getDataReturn == ErrorCode::EXCEPTION_ERROR) {
//            onErrorCallback("Error");
//            return ReturnCode::SUCCESS;
//        }
//    }
//    return ReturnCode::SUCCESS;
//}

IDataStorage::ReturnCode DataStorage::getState(const std::string& userSensorName, OnSuccessCallbackType onSuccessCallback,
                                               OnErrorCallbackType onErrorCallback)
{
    if (isConnected == false) {
        return ReturnCode::WRONG_CONFIG;
    }

    if (onErrorCallback == nullptr ||  onSuccessCallback == nullptr) {
        return ReturnCode::NULLPTR_CALLBACKS;
    }
    // get data
    std::string systemSensorName, sensorType;
    auto funcReturn = getSensorTypeAndSystemSensorName(userSensorName, systemSensorName, sensorType);
    if (funcReturn == ErrorCode::SUCCESS) {
        std::string state;
        auto getDataReturn = getData(systemSensorName, sensorType, state);
//      IF SUCCESS OF getData
        if (getDataReturn == ErrorCode::SUCCESS) {
            onSuccessCallback(state);
            return ReturnCode::SUCCESS;
        }
//      IF ERROR of getData
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
        if (getDataReturn == ErrorCode::WRONG_SENSOR_TYPE) {
            onErrorCallback("WRONG SENSOR TYPE");
            return ReturnCode::SUCCESS;

        }
    }
//     if Error of getSensorTypeAndSystemSensorName
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

// Сейчас Данный метод очень короткий и простой, но не как не проверяет то что данные введены верно!.
//IDataStorage::ReturnCode DataStorage::setStateByClientQuery(const std::string& systemSensorName,
//                                                            const std::string& sensorType,
//                                                            const std::string& newState,
//                                                            OnSuccessCallbackType onSuccessCallback,
//                                                            OnErrorCallbackType onErrorCallback)
//{
//    if (isConnected == false) {
//        return ReturnCode::WRONG_CONFIG;
//    }
//
//    if (onSuccessCallback == nullptr || onErrorCallback == nullptr) {
//        return ReturnCode::NULLPTR_CALLBACKS;
//    }
//
//    try {
//
//        std::string exec;
//        exec = "update " + sensorType + " set state='" + newState + "' where systemSensorName='" + systemSensorName + "'";
//        db->execute(exec);
//        return ReturnCode::SUCCESS;
////        if (sensorType == sensorTypeConstants.manyStatesType) {
////            auto tabManyStatesType = DataDB::ManyStatesType();
////            db->operator()(update(tabManyStatesType).set(tabManyStatesType.state = newState)
////                                   .where(tabManyStatesType.systemSensorName == systemSensorName));
////            onSuccessCallback(nullptr);
////            return ReturnCode::SUCCESS;
////        }
//
////        db->execute("UPDATE BinaryType SET state='off' WHERE (BinaryType.systemSensorName='light1')");
//
////        if (sensorType == sensorTypeConstants.binaryType) {
////            auto tabBinaryType = DataDB::BinaryType();
//////            db->update()
////            db->operator()(sqlpp::update(tabBinaryType).set(tabBinaryType.state = "ON")
////                                   .where(tabBinaryType.systemSensorName =="light1"));
////            onSuccessCallback(nullptr);
////            return ReturnCode::SUCCESS;
////        }
//
////        if (sensorType == sensorTypeConstants.monitorType) {
////            auto tabMonitorType = DataDB::MonitorType();
////            db->operator()(update(tabMonitorType).set(tabMonitorType.state = newState)
////                                   .where(tabMonitorType.systemSensorName == systemSensorName));
////            onSuccessCallback(nullptr);
////            return ReturnCode::SUCCESS;
////        }
//        // Дальше дергается колбэк ошибки
//    } catch(std::exception& e) {
//        onErrorCallback(e.what());
//    }
//    onErrorCallback("error");
//    return ReturnCode::SUCCESS;
//}

IDataStorage::ReturnCode DataStorage::addSensor(const std::string& userSensorName, const std::string& systemSensorName,
                                                const std::string& sensorType, const std::string& FAid, const std::string& kindOfSensor,
                                                const std::string& startingState,
                                                OnSuccessCallbackType onSuccessCallback, OnErrorCallbackType onErrorCallback)
{
    if (isConnected == false) {
        return ReturnCode::WRONG_CONFIG;
    }
    if (onSuccessCallback ==  nullptr  || onErrorCallback == nullptr) {
        return ReturnCode::NULLPTR_CALLBACKS;
    }
    {
        int inc = 0;
        const auto tabMainTable = DataDB::MainTable();
        for (const auto &row : db->operator()(sqlpp::select(tabMainTable.systemSensorName).from(tabMainTable).
                where(tabMainTable.userSensorName == userSensorName))) {
            ++inc;
            break;
        }
        if (inc > 0) {
            onErrorCallback("Such user sensor name is already used.");
            return ReturnCode::SUCH_US_NAME_IS_USED;
        }
        inc = 0;
        for (const auto &row : db->operator()(sqlpp::select(tabMainTable.sensorType).from(tabMainTable).
                where(tabMainTable.systemSensorName == systemSensorName))) {
            ++inc;
            break;
        }
        if (inc > 0) {
            onErrorCallback("Such system sensor name is already used.");
            return ReturnCode::SUCH_SS_NAME_IS_USED;
        }

    }
    if (sensorType != sensorTypeConstants.binaryType && sensorType != sensorTypeConstants.monitorType && sensorType != sensorTypeConstants.manyStatesType) {
        return ReturnCode::WRONG_SENSOR_TYPE;
    }

    try {
        auto tabMainTable = DataDB::MainTable();
        db->operator()(insert_into(tabMainTable).set(tabMainTable.userSensorName = userSensorName,
                                                     tabMainTable.systemSensorName = systemSensorName,
                                                     tabMainTable.sensorType = sensorType));

        if (sensorType == sensorTypeConstants.binaryType) {
            auto tabBinaryType = DataDB::BinaryType();
            db->operator()(insert_into(tabBinaryType).set(tabBinaryType.systemSensorName = systemSensorName, tabBinaryType.type = kindOfSensor, tabBinaryType.FAId = FAid,
                                                                                                                                    tabBinaryType.state = startingState));
            onSuccessCallback("Sensor added");
            return ReturnCode::SUCCESS;
        }

        if (sensorType == sensorTypeConstants.monitorType) {
            auto tabMonitorType = DataDB::MonitorType();
            db->operator()(insert_into(tabMonitorType).set(tabMonitorType.systemSensorName = systemSensorName, tabMonitorType.type = kindOfSensor, tabMonitorType.FAId = FAid,
                                                          tabMonitorType.state = startingState));
            onSuccessCallback("Sensor added");
            return ReturnCode::SUCCESS;
        }

        if (sensorType == sensorTypeConstants.manyStatesType) {
            auto tabManyStatesType = DataDB::BinaryType();
            db->operator()(insert_into(tabManyStatesType).set(tabManyStatesType.systemSensorName = systemSensorName, tabManyStatesType.type = kindOfSensor, tabManyStatesType.FAId = FAid,
                                                          tabManyStatesType.state = startingState));
            onSuccessCallback("Sensor added");
            return ReturnCode::SUCCESS;
        }
    } catch (std::exception& e) {
        onErrorCallback(e.what());
        return ReturnCode::EXCEPTION_ERROR;
    }
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::removeSensor(const std::string& systemSensorName, OnSuccessCallbackType onSuccessCallback,
                                                   OnErrorCallbackType onErrorCallback)
{
    if (isConnected == false) {
        return ReturnCode::WRONG_CONFIG;
    }

    if (onSuccessCallback ==  nullptr  || onErrorCallback == nullptr) {
        return ReturnCode::NULLPTR_CALLBACKS;
    }
    std::string sensorType;
    try {
            int inc = 0;
            const auto tabMainTable = DataDB::MainTable();
            for (const auto &row : db->operator()(sqlpp::select( tabMainTable.sensorType).from(tabMainTable)
                                                          .where(tabMainTable.systemSensorName == systemSensorName))) {
                inc++;
                sensorType = row.sensorType;
            }
            if (inc == 0) {
                onErrorCallback("NO_SUCH_SS_NAME");
                return ReturnCode::NO_SUCH_SS_NAME;
            } else if (inc > 1) {
                onErrorCallback("TOO MANY SS NAMES");
                return ReturnCode::TOO_MANY_SS_NAMES;
            }
    } catch (std::exception& e) {
        onErrorCallback("EXCEPTION");
        return ReturnCode::EXCEPTION_ERROR;
    }

    try {
        auto tabMainTable = DataDB::MainTable();
        db->operator()(remove_from(tabMainTable).where(tabMainTable.systemSensorName == systemSensorName));
        if (sensorType ==   sensorTypeConstants.binaryType) {
            auto tabBinaryType = DataDB::BinaryType();
            db->operator()(remove_from(tabBinaryType).where(tabBinaryType.systemSensorName == systemSensorName));
        } else if (sensorType == sensorTypeConstants.manyStatesType) {
            auto tabManyStatesType = DataDB::ManyStatesType();
            db->operator()(remove_from(tabManyStatesType).where(tabManyStatesType.systemSensorName == systemSensorName));
        } else if (sensorType ==  sensorTypeConstants.monitorType) {
            auto tabMonitorType = DataDB::MonitorType();
            db->operator()(remove_from(tabMonitorType).where(tabMonitorType.systemSensorName == systemSensorName));
        } else {
            onErrorCallback("No such sensor type");
            return ReturnCode::WRONG_SENSOR_TYPE;
        }
    } catch (std::exception& e) {
        onErrorCallback(e.what());
        return ReturnCode::EXCEPTION_ERROR;
    }
    onSuccessCallback("remove - success");
    return ReturnCode::SUCCESS;


//    db(remove_from(tab).where(tab.alpha == tab.alpha + 3));

}

IDataStorage::ReturnCode DataStorage::getSystemSensorNameByUSName(const std::string& userSensorName, std::string& systemSensorName)
{
    if (isConnected == false) {
        return ReturnCode::WRONG_CONFIG;
    }

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
        return ReturnCode::NO_SUCH_US_NAME;
    } else if (inc > 1) {
        return ReturnCode::TOO_MANY_US_NAMES;
    }
    return ReturnCode::SUCCESS;
}
IDataStorage::ReturnCode DataStorage::getUserSensorNameBySSName(const std::string &systemSensorName,
                                                                std::string &userSensorName)
{
    if (isConnected == false) {
        return ReturnCode::WRONG_CONFIG;
    }

    int inc = 0;
    const auto tabMainTable  = DataDB::MainTable();
    std::string uSName;
    for (const auto& row : db->operator()(sqlpp::select(tabMainTable.userSensorName).from(tabMainTable).
            where(tabMainTable.systemSensorName == systemSensorName))) {
        ++inc;
        uSName  = row.userSensorName;
    }
    if (inc == 1) {
        userSensorName = uSName;
        return ReturnCode::SUCCESS;
    } else if (inc == 0 ) {
        return ReturnCode::NO_SUCH_SS_NAME;
    } else if (inc > 1) {
        return ReturnCode::TOO_MANY_SS_NAMES;
    }
    return ReturnCode::SUCCESS;
}

IDataStorage::ReturnCode DataStorage::getFAid(const std::string& userSensorName, std::string& FAid)
{
    if (isConnected == false) {
        return ReturnCode::WRONG_CONFIG;
    }
    std::string systemSensorName, sensorType;
    auto funcReturn = getSensorTypeAndSystemSensorName(userSensorName, systemSensorName, sensorType);
    if (funcReturn == ErrorCode::SUCCESS) {
        std::string id;
//        try {
            int inc = 0;
            if (sensorType == sensorTypeConstants.binaryType) {
                auto tabBinaryType = DataDB::BinaryType();
                for (const auto &row : db->operator()(sqlpp::select(tabBinaryType.FAId)
                                                              .from(tabBinaryType).where(
                        tabBinaryType.systemSensorName == systemSensorName))) {
                    id = row.FAId;
                    ++inc;
                }
            } else if (sensorType == sensorTypeConstants.manyStatesType) {
                auto tabManyStatesType = DataDB::ManyStatesType();
                for (const auto &row : db->operator()(sqlpp::select(tabManyStatesType.FAId)
                                                              .from(tabManyStatesType).where(
                        tabManyStatesType.systemSensorName == systemSensorName))) {
                    id = row.FAId;
                    ++inc;
                }
            } else if (sensorType == sensorTypeConstants.monitorType) {
                auto tabMonitorType = DataDB::MonitorType();
                for (const auto &row : db->operator()(sqlpp::select(tabMonitorType.FAId)
                                                              .from(tabMonitorType).where(
                        tabMonitorType.systemSensorName == systemSensorName))) {
                    id = row.FAId;
                    ++inc;
                }
            } else {
                return ReturnCode::WRONG_SENSOR_TYPE;
            }
//        } catch (std::exception& e) {
//            return ReturnCode::EXCEPTION_ERROR;
//        }
        if (inc == 1) {
            FAid = id;
            return ReturnCode::SUCCESS;
        } else if (inc == 0) {
            return ReturnCode::NO_SUCH_SS_NAME;
        } else if (inc > 1) {
            return ReturnCode::TOO_MANY_SS_NAMES;
        }
//  Main IF;
    } else {
        if (funcReturn == ErrorCode::THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB) {
            return ReturnCode::TOO_MANY_US_NAMES;
        }

        if (funcReturn == ErrorCode::NO_SUCH_USER_SENSOR_NAME_IN_DB) {
            return ReturnCode::NO_SUCH_US_NAME;
        }
    }
    return ReturnCode::SUCCESS;
}

/*
 *
 * ДАЛЬШЕ ПРИВАТНЫЕ МЕТОДЫ!
 *
 *
 */

IDataStorage::ErrorCode DataStorage::getSensorTypeAndSystemSensorName(const std::string& userSensorName,
                                                                      std::string &systemSensorName,
                                                                      std::string &sensorType)
{

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
    return ErrorCode::SUCCESS;
}


IDataStorage::ErrorCode DataStorage::getData(const std::string& systemSensorName,
                                             const std::string& sensorType, std::string& currentState)
{

    int inc = 0;
    std::string data;
    try {

        if (sensorType == sensorTypeConstants.binaryType) {
            auto tabBinaryType = DataDB::BinaryType();
            for (const auto &row : db->operator()(sqlpp::select(tabBinaryType.state).from(tabBinaryType).
                    where(tabBinaryType.systemSensorName == systemSensorName))) {
                data = row.state;
                ++inc;
            }

        } else if (sensorType == sensorTypeConstants.manyStatesType) {
            auto tabManyStatesType = DataDB::ManyStatesType();
            for (const auto &row : db->operator()(sqlpp::select(tabManyStatesType.state).from(tabManyStatesType).
                    where(tabManyStatesType.systemSensorName == systemSensorName))) {
                data = row.state;
                ++inc;
            }

        } else if (sensorType == sensorTypeConstants.monitorType) {
            auto tabMonitorType = DataDB::MonitorType();
            for (const auto &row : db->operator()(sqlpp::select(tabMonitorType.state).from(tabMonitorType).
                    where(tabMonitorType.systemSensorName == systemSensorName))) {
                data = row.state;
                ++inc;
            }
        } else {
            return ErrorCode::WRONG_SENSOR_TYPE;
        }
        // SUCCESS just if inc = 1, coze in it turn was just 1 FOR iteration.
        if (inc == 0) {
            return ErrorCode::THERE_ARE_NOT_SUCH_SYSTEM_SENSOR_NAMES_IN_DB;
        } else if (inc > 1) {
            return ErrorCode::THERE_ARE_TOO_MANY_SYSTEM_SENSOR_NAMES_IN_DB;
        } else if (inc == 1) {
            currentState = data;
            return ErrorCode::SUCCESS;
        }
    } catch (std::exception& ) {
        return ErrorCode::EXCEPTION_ERROR;
    }
    return ErrorCode::EXCEPTION_ERROR;
}




//TODO ПЕРЕДЕЛАТЬ setStateByClientQuery, т.к. update не работает. попробовать использовать execute.
//TODO Подумать, что передавать как дату в Success
