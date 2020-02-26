//
// Created by localcc on 2/12/20.
//

#ifndef PASSXTENDED_SERVER_CONFIG_H
#define PASSXTENDED_SERVER_CONFIG_H
#include <string>
#include <filesystem>
static const int port = 1150;
static const std::filesystem::path folder_path = "/home/localcc/.password-store";
static const std::filesystem::path storage_folder = "/etc/pxtended_server";
static char* KEY_ID = "2E0C7AF4F803AC64";
static std::string export_command = "gpg --export";
static const unsigned char* CERT_C = reinterpret_cast<const unsigned char*>("CA");
static const unsigned char* CERT_O = reinterpret_cast<const unsigned char*>("localcc");
static const unsigned char* CERT_CN = reinterpret_cast<const unsigned char*>("Danil");
#endif //PASSXTENDED_SERVER_CONFIG_H
