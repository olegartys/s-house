//
// Created by eninng on 29.03.16.
//

#ifndef SHOUSEDATASTORAGE_DATASTORAGE_H
#define SHOUSEDATASTORAGE_DATASTORAGE_H
// This class depends on library SQLPP11, which in turn depends on the library MYSQL-CLIENT.

#include "IDataStorage.h"
#include "DataDB.h"

#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include "../../external/ThreadPool/ThreadPool.h"

class DataStorage final : public IDataStorage {
public:
    DataStorage() = default;

    // Prevent copy of the objects.
    DataStorage(const DataStorage&) = delete;
    DataStorage& operator= (const DataStorage&) = delete;

    ~DataStorage() = default;

    ReturnCode connect(const std::string& userName = "root", const std::string& password = "root",
                                    const std::string& DBname = "DataStorage") override;

//    ReturnCode setStateByUserQuery(const std::string& userSensorName, const std::string& newState,
//                                                                        OnStateChangedCallbackType,
//                                                                        OnOldStateCallbackType,
//                                                                        OnErrorCallbackType) override;

    ReturnCode setState(const std::string& systemSensorName, const std::string& newState, OnSetStateSuccessCallbackType, OnErrorCallbackType) override;

    ReturnCode getState(const std::string& userSensorName, OnSuccessCallbackType, OnErrorCallbackType) override;

//    ReturnCode setStateByClientQuery(const std::string& systemSensorName, const std::string& sensorType,
//                                        const std::string& newState,
//                                        OnSuccessCallbackType,
//                                        OnErrorCallbackType) override;

    ReturnCode addSensor(const std::string& userSensorName, const std::string& systemSensorName,
                         const std::string& sensorType, const std::string& FAid, const std::string& kindOfSensor,
                         const std::string& startingState,
                         OnSuccessCallbackType, OnErrorCallbackType) override;

    ReturnCode removeSensor(const std::string& systemSensorName, OnSuccessCallbackType, OnErrorCallbackType) override;
    /*
     *
     */
    ReturnCode getFAid(const std::string& userSensorName, std::string& FAid) override;

    ReturnCode getSystemSensorNameByUSName(const std::string& userSensorName, std::string& systemSensorName) override;
    ReturnCode getUserSensorNameBySSName(const std::string& systemSensorName, std::string& userSensorName) override;


private:
    // getData  это аналог getState, необходимый для внутреннего получения состояния датчика
    /*
     * @brief Method getData return IDAtaStorage::ErrorCode.
     * This method accept next parameters:
     * @param[in] systemSensorName - std::string with system Sensor Name;
     * @param[in] sensorType - std::string with type of sensor;
     * @param[in] currentState - std::string& reference on std::string in which current state will be written; *
     */

    ErrorCode getData(const std::string& systemSensorName,const std::string& sensorType, std::string& currentState);
    /*
     * @brief Method getSensorTypeAndSystemSensorName get Type of sensor (Binary, ManyStates or Monitor Type) and System sensor name by User Sensor Name from DB.
     * This method return IDataStorage::ErrorCode.
     * It return Success if there are only 1 such User Sensor Name in DB.
     * It return NO_SUCH_USER_SENSOR_NAME_IN_DB if there aren't such User sensor names.
     * And it return THERE_ARE_TOO_MANY_USER_SENSOR_NAMES_IN_DB if there more than 1 such User Sensor Name.
     */
    ErrorCode getSensorTypeAndSystemSensorName(const std::string& userSensorName,
                                               std::string& systemSensorName, std::string& sensorType);

    bool isConnected = false;

    /*
     * @brief Shared_ptr on config.
     */
    const std::shared_ptr<sqlpp::mysql::connection_config> config = std::make_shared<sqlpp::mysql::connection_config>();
    /*
     * @brief Shared_ptr on DB.
     */
    std::shared_ptr<sqlpp::mysql::connection> db;
    SensorType sensorTypeConstants;
    std::unique_ptr<ThreadPool>  dsThreadPool;
};


#endif //SHOUSEDATASTORAGE_DATASTORAGE_H
