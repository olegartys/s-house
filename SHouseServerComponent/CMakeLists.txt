cmake_minimum_required(VERSION 3.3)
project(SHouseServerComponent)

# Set compilaton flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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
add_executable(SHouseServerComponent main.cpp)