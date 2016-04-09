cmake_minimum_required(VERSION 3.3)
project(SHouseServerComponent)

# Set compilaton flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

# Link with libraries
link_directories(/usr/lib)
link_directories(/usr/local/lib)

# QueryAnalyzingPacket adding.
add_subdirectory(QueryAnalyzingPacket)

# MessagePacket adding.
add_subdirectory(MessagePacket)

# DataStoragePacket adding.
add_subdirectory(DataStoragePacket)

# Main output artefact. [now for TEST]
add_executable(SHouseServerComponent main.cpp main.cpp)

# Link internal packets (libraries) to main artefact.
target_link_libraries(SHouseServerComponent DataStoragePacket)
target_link_libraries(SHouseServerComponent MessagePacket)
target_link_libraries(SHouseServerComponent QueryAnalyzingPacket)

# Add external libraries to Main executable artefact.
#target_link_libraries(SHouseServerComponent jsoncpp)
target_link_libraries(SHouseServerComponent sqlpp-mysql)
target_link_libraries(SHouseServerComponent mysqlclient)
target_link_libraries(SHouseServerComponent event)

