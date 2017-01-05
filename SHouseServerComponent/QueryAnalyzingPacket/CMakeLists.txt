project(QueryAnalyzingPacket)

# Set package sources
set(SOURCE_FILES src/QueryListener/QueryListener.cpp include/QueryListener/QueryListener.h include/IListener.h include/QueryHandler.h include/IActor.h src/QueryHandler.cpp ../CommonPacket/include/FunctionTraits.h include/IHandler.h ../CommonPacket/include/Utils.h ../CommonPacket/src/Utils.cpp src/QueryActor.cpp include/QueryActor.h src/QueryActor.cpp include/QueryActor.h src/QueryHandler.cpp src/QueryListener/ConnectionWrapper.cpp include/QueryListener/ConnectionWrapper.h)

# Create an executable for QueryAnalyzingPacket
add_library(QueryAnalyzingPacket SHARED ${SOURCE_FILES})

# Set directory with include files
target_include_directories(QueryAnalyzingPacket PUBLIC include/)

# Link it with MessagePacket library
target_link_libraries(QueryAnalyzingPacket LINK_PUBLIC MessagePacket)

# FIXME: this shouldn't be here
target_include_directories(QueryAnalyzingPacket PUBLIC ../external/sqlpp11/sqlpp11/include/)
target_include_directories(QueryAnalyzingPacket PUBLIC ../external/sqlpp11/date/)
target_include_directories(QueryAnalyzingPacket PUBLIC ../external/sqlpp11/sqlpp11-connector-mysql/include)
