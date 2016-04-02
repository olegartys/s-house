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

    int createConnectionWithSqlDB(std::string userName = "root", std::string password = "root", std::string DBname = "DataStorage") override;

    int setStateByUserQuery(std::string userSensorName, std::string newState, OnStateChangedCallbackType,
                                         OnOldStateCallbackType,
                                         OnErrorCallbackType) override;


    int getState(std::string userSensorName, OnSuccessCallbackType, OnErrorCallbackType);

    int setStateByClientQuery(std::string systemSensorName, std::string sensorType,
                                      OnSuccessCallbackType,
                                      OnErrorCallbackType);

    int getSystemSensorNameByUserSensorName(std::string userSensorName, std::string& systemSensorName) override; // ???? НУЖНО ЛИ?

    int addSensor();

    int removeSensor();

private:
    // getData  это аналог getState, необходимый для внутреннего получения состояния датчика

    int getData(std::string systemSensorName, std::string sensorType, std::string& currentState);
    int getSensorTypeAndSystemSensorName(std::string userSensorName, std::string& systemSensorName, std::string& sensorType);

};


#endif //SHOUSEDATASTORAGE_DATASTORAGE_H
