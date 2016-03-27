cmake_minimum_required(VERSION 3.3)
project(QueryAnalyzingPacket)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp QueryListener.cpp QueryListener.h IListener.h IParser.h SensorInfo.h SensorName.h QueryHandler.h IActor.h QueryHandler.cpp FunctionTraits.h QueryParser.cpp QueryParser.h IHandler.h Utils.h Utils.cpp)
add_executable(QueryAnalyzingPacket ${SOURCE_FILES})