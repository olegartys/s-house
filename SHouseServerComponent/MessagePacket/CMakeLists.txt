project(MessagePacket)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCES src/Message.cpp lib/jsoncpp.cpp src/Response.cpp)

add_library(MessagePacket ${SOURCES})

target_link_libraries(MessagePacket)

target_include_directories(MessagePacket PUBLIC lib/)
target_include_directories(MessagePacket PUBLIC include/)