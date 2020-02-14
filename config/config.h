//
// Created by localcc on 2/12/20.
//

#ifndef PASSXTENDED_SERVER_CONFIG_H
#define PASSXTENDED_SERVER_CONFIG_H
#include <string>
#include <filesystem>
static const char* folder_path = "/home/localcc/.password-store";
static const std::filesystem::path storage_folder = "/etc/pxtended_server";
static const unsigned char* CERT_C = reinterpret_cast<const unsigned char*>("CA");
static const unsigned char* CERT_O = reinterpret_cast<const unsigned char*>("localcc");
static const unsigned char* CERT_CN = reinterpret_cast<const unsigned char*>("Danil");
#endif //PASSXTENDED_SERVER_CONFIG_H
