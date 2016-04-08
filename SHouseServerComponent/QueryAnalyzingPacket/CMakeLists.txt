project(QueryAnalyzingPacket)

# Set package sources
set(SOURCE_FILES src/QueryListener.cpp include/QueryListener.h include/IListener.h include/SensorInfo.h include/SensorName.h include/QueryHandler.h include/IActor.h src/QueryHandler.cpp include/FunctionTraits.h include/IHandler.h include/Utils.h src/Utils.cpp src/QueryActor.cpp include/QueryActor.h src/QueryActor.cpp include/QueryActor.h src/QueryHandler.cpp)

# Create an executable for QueryAnalyzingPacket
add_library(QueryAnalyzingPacket SHARED ${SOURCE_FILES})

# Set directory with include files
target_include_directories(QueryAnalyzingPacket PUBLIC include/)

# Link it with MessagePacket library
target_link_libraries(QueryAnalyzingPacket LINK_PUBLIC MessagePacket)
