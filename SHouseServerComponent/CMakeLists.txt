cmake_minimum_required(VERSION 3.3)

# Set compilaton flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(SHouseServerComponent)

# QueryAnalyzingPacket adding.
add_subdirectory(QueryAnalyzingPacket)

# MessagePacket adding.
add_subdirectory(MessagePacket)