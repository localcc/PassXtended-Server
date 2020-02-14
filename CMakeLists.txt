cmake_minimum_required(VERSION 3.15)
project(PassXtended_Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config)

set(FLATBUFFER_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/flatbuffer)

find_package(FlatBuffers REQUIRED)
find_package(OpenSSL REQUIRED)

set(tcp_server_src src/tcp_server/tcp_server.cpp include/tcp_server/tcp_server.h include/tcp_server/tcp_client.h src/tcp_server/tcp_client.cpp)
set(filesystem_helper_src include/filesystem_helper/filesystem_helper.h src/filesystem_helper/filesystem_helper.cpp)
set(flatbuf_src src/flatbuffers/files.fbs src/flatbuffers/file.fbs)
set(tls_helper_src src/tls_helper/tls_helper.cpp include/tls_helper/tls_helper.h)
set(config_headers config/config.h)

# 0 for ARGN to loop through files properly
FLATBUFFERS_GENERATE_C_HEADERS(0 ${flatbuf_src})

add_executable(${PROJECT_NAME} ${config_headers} ${flatbuf_src} main.cpp ${tcp_server_src} ${filesystem_helper_src} ${tls_helper_src})
add_dependencies(${PROJECT_NAME} files_generated.h file_generated.h)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto OpenSSL::SSL)
