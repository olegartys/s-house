project(DataStoragePacket)

set(SOURCES src/DataStorage.cpp)

add_library(DataStoragePacket SHARED ${SOURCES})

target_include_directories(DataStoragePacket PUBLIC include/)
