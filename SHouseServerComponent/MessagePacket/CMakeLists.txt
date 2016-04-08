project(MessagePacket)

set(SOURCES src/Message.cpp lib/jsoncpp.cpp src/Response.cpp include/Response.h)

add_library(MessagePacket ${SOURCES})
target_link_libraries(MessagePacket)

target_include_directories(MessagePacket PUBLIC include/)
target_include_directories(MessagePacket PUBLIC lib/json/)