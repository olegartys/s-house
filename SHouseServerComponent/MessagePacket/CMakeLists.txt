project(MessagePacket)

set(SOURCES src/Message.cpp)

add_library(MessagePacket SHARED ${SOURCES})

target_include_directories(MessagePacket PUBLIC include/)