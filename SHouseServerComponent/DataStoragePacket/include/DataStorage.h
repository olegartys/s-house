//
// Created by eninng on 29.03.16.
//

#ifndef SHOUSEDATASTORAGE_DATASTORAGE_H
#define SHOUSEDATASTORAGE_DATASTORAGE_H


#include "IDataStorage.h"
#include "DataDB.h"

class DataStorage : public IDataStorage {
public:
    DataStorage() = default;

    // Prevent copy of the objects.
    DataStorage(const DataStorage&) = delete;
    DataStorage& operator= (const DataStorage&) = delete;

    ~DataStorage() = default;

    ReturnCode createConnectionWithSqlDB(std::string userName = "root", std::string password = "root", std::string DBname = "DataStorage") override;

    ReturnCode setStateByUserQuery(std::string userSensorName, std::string newState, OnStateChangedCallbackType,
                                         OnOldStateCallbackType,
                                         OnErrorCallbackType) override;


    ReturnCode getState(std::string userSensorName, OnSuccessCallbackType, OnErrorCallbackType);

    ReturnCode setStateByClientQuery(std::string systemSensorName, std::string sensorType,
                                      OnSuccessCallbackType,
                                      OnErrorCallbackType);

    ReturnCode addSensor();

    ReturnCode removeSensor();

    ReturnCode getSystemSensorNameByUserSensorName(std::string userSensorName, std::string& systemSensorName);
private:
    // getData  это аналог getState, необходимый для внутреннего получения состояния датчика

    ErrorCode getData(std::string systemSensorName, std::string sensorType, std::string& currentState);
    ErrorCode getSensorTypeAndSystemSensorName(std::string userSensorName, std::string& systemSensorName, std::string& sensorType);
     // ???? НУЖНО ЛИ?

};


#endif //SHOUSEDATASTORAGE_DATASTORAGE_H
