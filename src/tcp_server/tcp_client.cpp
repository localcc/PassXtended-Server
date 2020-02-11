//
// Created by localcc on 2/4/20.
//
#include <tcp_server/tcp_client.h>
#include <filesystem_helper/filesystem_helper.h>
#include <flatbuffers/flatbuffers.h>
#include <flatbuffer/files_generated.h>
#include <thread>
#include <unistd.h>
#include <cstdio>


using namespace passxtended::protos;

tcp_client::tcp_client(int client_sockfd) {
    this->sock_fd = client_sockfd;
}

tcp_client::~tcp_client() {
    this->sock_fd = 0;
}

enum Commands {
    file_fetch = 0,
    file_write = 1,
    totp_req = 2
};

void tcp_client::handle() {
    int len = 0;
    do {
        char command[1];
        len = read(this->sock_fd, command, 1);
        if(command[0] == file_fetch) {
            printf("Fetching file!\n");
            flatbuffers::FlatBufferBuilder builder;
            auto raw_files = filesystem_helper::get_instance()->read_all_files();
            std::vector<flatbuffers::Offset<file>> files_vector;
            for(const auto& file : raw_files) {
                auto data_offset = builder.CreateVector(std::get<1>(file), std::get<2>(file));

                auto filename_offset = builder.CreateString(std::get<0>(file));
                auto file_offset = Createfile(builder, data_offset, filename_offset);
                files_vector.push_back(file_offset);
            }
            auto files = builder.CreateVector(files_vector);
            auto all_files = Createfiles(builder, files);
            builder.Finish(all_files);
            write(this->sock_fd, builder.GetCurrentBufferPointer(), builder.GetSize());
        }else if(command[0] == file_write) {
            flatbuffers::FlatBufferBuilder builder;
            int data_size = 0;
            auto* data_size_raw = new unsigned char[4];
            read(this->sock_fd, data_size_raw, 4);
            data_size = data_size_raw[3] << 24 | data_size_raw[2] << 16 | data_size_raw[1] << 8 | data_size_raw[0];
            free(data_size_raw);
            auto* data = new unsigned char[data_size];
            read(this->sock_fd, data, data_size);
            auto file = Getfile(data);
            filesystem_helper::get_instance()->write_file(file->filename()->str(),
                                                          reinterpret_cast<const char *>(file->data()->data()), file->data()->size());
        }else if(command[0] == totp_req) {
            //TODO: totp
            printf("Request for totp!\n");
        }
    }while(len);
}