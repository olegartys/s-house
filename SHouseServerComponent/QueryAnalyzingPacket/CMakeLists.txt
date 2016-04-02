project(QueryAnalyzingPacket)

# Set package sources
set(SOURCE_FILES main.cpp src/QueryListener.cpp include/QueryListener.h include/IListener.h include/IParser.h include/SensorInfo.h include/SensorName.h include/QueryHandler.h include/IActor.h src/QueryHandler.cpp include/FunctionTraits.h src/QueryParser.cpp include/QueryParser.h include/IHandler.h include/Utils.h src/Utils.cpp)

# Create an executable for QueryAnalyzingPacket
add_executable(QueryAnalyzingPacket ${SOURCE_FILES})

# Set directory with include files
target_include_directories(QueryAnalyzingPacket PUBLIC include/)

# Link with libraries
link_directories(/usr/lib)
link_directories(/usr/local/lib)

# Link it with MessagePacket library
target_link_libraries(QueryAnalyzingPacket LINK_PUBLIC MessagePacket crafter)
