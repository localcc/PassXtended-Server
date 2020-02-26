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
#include <config.h>
#include <iostream>


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
    cert_fetch = 2,
    totp_req = 3
};

void tcp_client::handle() {
    int len = 0;
    do {
        char command[1];
        len = SSL_read(this->ssl, command, 1);
        if(len < 1) break;
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
            data_size = read_int_from_sockfd();
            
            auto* data = new unsigned char[data_size];
            SSL_read(this->ssl, data, data_size);
            auto file = Getfile(data);
            filesystem_helper::get_instance()->write_file(file->filename()->str(),
                                                          reinterpret_cast<const char *>(file->data()->data()), file->data()->size());
        }else if(command[0] == cert_fetch) {
            printf("Fetching certificate!\n");
            flatbuffers::FlatBufferBuilder builder;

            //Launching gpg to get the encrypted key
            std::vector<int8_t> all_data;
            
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(export_command.append(KEY_ID).c_str(), "r"), pclose);
            int8_t* data_arr = new int8_t[1024];
            //Clean
            memset(data_arr, 0, 1024);
            int read = 0;
            while((read = fread(data_arr, 1, 1024, pipe.get())) > 0) {
                all_data.insert(all_data.end(), data_arr, data_arr + read);
            }

            auto data_offset = builder.CreateVector(all_data);
            auto filename_offset = builder.CreateString("");
            auto file_offset = Createfile(builder, data_offset, filename_offset);
            builder.Finish(file_offset);
            write_int_to_sockfd(builder.GetSize());
            SSL_write(this->ssl, builder.GetBufferPointer(), builder.GetSize());

        }else if(command[0] == totp_req) {
            //TODO: totp
            printf("Request for totp!\n");
        }
    }while(len > 0);
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
    delete size_arr;
}

unsigned int tcp_client::read_int_from_sockfd() {
    auto* size_arr = new char[4];
    SSL_read(this->ssl, size_arr, 4);
    int val = size_arr[3] << 24 | size_arr[2] << 16 | size_arr[1] << 8 | size_arr[0];
    delete size_arr;
    return val;
}