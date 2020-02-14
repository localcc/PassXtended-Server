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

tcp_client::tcp_client(SSL* ssl) {
    this->ssl = ssl;
}

tcp_client::~tcp_client() {
    SSL_free(this->ssl);

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
        len = SSL_read(this->ssl, command, 1);
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
            write_int_to_sockfd(builder.GetSize());
            SSL_write(this->ssl, builder.GetCurrentBufferPointer(), builder.GetSize());
        }else if(command[0] == file_write) {
            flatbuffers::FlatBufferBuilder builder;
            int data_size = 0;
            auto* data_size_raw = new unsigned char[4];
            SSL_read(this->ssl, data_size_raw, 4);
            data_size = read_int_from_sockfd();
            free(data_size_raw);
            auto* data = new unsigned char[data_size];
            SSL_read(this->ssl, data, data_size);
            auto file = Getfile(data);
            filesystem_helper::get_instance()->write_file(file->filename()->str(),
                                                          reinterpret_cast<const char *>(file->data()->data()), file->data()->size());
        }else if(command[0] == totp_req) {
            //TODO: totp
            printf("Request for totp!\n");
        }
    }while(len);
    SSL_shutdown(ssl);
    this->~tcp_client();

}

void tcp_client::write_int_to_sockfd(unsigned int i) {
    auto* size_arr = new char[4];
    size_arr[3] = (i >> 24) & 0xFF;
    size_arr[2] = (i >> 16) & 0xFF;
    size_arr[1] = (i >> 8) & 0xFF;
    size_arr[0] = i & 0xFF;
    SSL_write(this->ssl, size_arr, 4);
}

unsigned int tcp_client::read_int_from_sockfd() {
    auto* size_arr = new char[4];
    SSL_read(this->ssl, size_arr, 4);
    return size_arr[3] << 24 | size_arr[2] << 16 | size_arr[1] << 8 | size_arr[0];
}