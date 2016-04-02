project(MessagePacket)

set(SOURCES src/Message.cpp)

add_library(MessagePacket SHARED ${SOURCES})

target_include_directories(MessagePacket PUBLIC include/)

# Link with libraries
link_directories(/usr/lib)
link_directories(/usr/local/lib)
target_link_libraries(MessagePacket crafter)
