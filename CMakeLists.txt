cmake_minimum_required(VERSION 3.15)
project(PassXtended_Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH cmake/modules/)
include_directories(include)

set(FLATBUFFER_BUILD_DIR include/flatbuffer)
message(${CMAKE_CURRENT_BINARY_DIR})
find_package(FlatBuffers REQUIRED)

set(tcp_server_src src/tcp_server/tcp_server.cpp include/tcp_server/tcp_server.h include/tcp_server/tcp_client.h src/tcp_server/tcp_client.cpp)
set(filesystem_helper_src include/filesystem_helper/filesystem_helper.h src/filesystem_helper/filesystem_helper.cpp)
set(flatbuf_src src/flatbuffers/files.fbs src/flatbuffers/file.fbs)

# 0 for ARGN to loop through files properly
FLATBUFFERS_GENERATE_C_HEADERS(0 ${flatbuf_src})

add_executable(PassXtended_Server ${flatbuf_src} main.cpp ${tcp_server_src} ${filesystem_helper_src})
add_dependencies(PassXtended_Server files_generated.h file_generated.h)
target_link_libraries(PassXtended_Server pthread)
