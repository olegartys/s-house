project(CommonPacket)

# Set the source files
set(SOURCE_FILES src/Utils.cpp)

# Create an library for QueryAnalyzingPacket
add_library(CommonPacket SHARED ${SOURCE_FILES})

# Set directory with include files
target_include_directories(CommonPacket PUBLIC include/)
