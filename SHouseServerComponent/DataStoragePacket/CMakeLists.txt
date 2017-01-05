project(DataStoragePacket)

set(SOURCES src/DataStorage.cpp)

add_library(DataStoragePacket SHARED ${SOURCES})

target_include_directories(DataStoragePacket PUBLIC include/)
target_include_directories(DataStoragePacket PUBLIC ../external/sqlpp11/sqlpp11/include/)
target_include_directories(DataStoragePacket PUBLIC ../external/sqlpp11/date/)
target_include_directories(DataStoragePacket PUBLIC ../external/sqlpp11/sqlpp11-connector-mysql/include)
#target_include_directories(DataStoragePacket PUBLIC ../external/ThreadPool/)
target_link_libraries(DataStoragePacket pthread)