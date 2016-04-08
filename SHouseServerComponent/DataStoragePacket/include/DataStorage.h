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


    ReturnCode getState(std::string userSensorName, OnSuccessCallbackType, OnErrorCallbackType) override;

    ReturnCode setStateByClientQuery(std::string systemSensorName, std::string sensorType, std::string newState,
                                      OnSuccessCallbackType,
                                      OnErrorCallbackType);

    ReturnCode addSensor();

    ReturnCode removeSensor();
    /*
     *
     */
    ReturnCode getFAid(std::string userSensorName, std::string& FAid) override;

    ReturnCode getSystemSensorNameByUserSensorName(std::string userSensorName, std::string& systemSensorName) override;

private:
    // getData  это аналог getState, необходимый для внутреннего получения состояния датчика
    /*
     * @brief Method getData return IDAtaStorage::ErrorCode.
     * This method accept next parameters:
     * @param[in] systemSensorName - std::string with system Sensor Name;
     * @param[in] sensorType - std::string with type of sensor;
     * @param[in] currentState - std::string& reference on std::string in which current state will be written; *
     */

    ErrorCode getData(std::string systemSensorName, std::string sensorType, std::string& currentState);
    /*
     * @brief Method getSensorTypeAndSystemSensorName get Type of sensor (Binary, ManyStates or Monitor Type) and System sensor name by User Sensor Name from DB.
     * This method return IDataStorage::ErrorCode.
     * It return Success if there are only 1 such User Sensor Name in DB.
     * It return NO_SUCH_USER_SENSOR_NAME_IN_DB if there aren't such User sensor names.
     * And it return THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB if there more than 1 such User Sensor Name.
     */
    ErrorCode getSensorTypeAndSystemSensorName(std::string userSensorName, std::string& systemSensorName, std::string& sensorType);
};


#endif //SHOUSEDATASTORAGE_DATASTORAGE_H
