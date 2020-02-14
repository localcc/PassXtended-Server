//
// Created by localcc on 2/5/20.
//

#ifndef PASSXTENDED_SERVER_FILESYSTEM_HELPER_H
#define PASSXTENDED_SERVER_FILESYSTEM_HELPER_H
#include <filesystem>
#include <vector>
#include <tuple>
#include <mutex>
#include <string>

class filesystem_helper {
public:
    static filesystem_helper* get_instance() { return current_instance; }
    filesystem_helper(std::string dir_path);
    //                      filename    data   size
    std::vector<std::tuple<std::string, int8_t*, int>> read_all_files();
    static void write_file(const std::string& file_name, const char* data, int size);

    ~filesystem_helper();
private:
    std::mutex mutex;
    std::string dir_path;
    static filesystem_helper* current_instance;
};

#endif //PASSXTENDED_SERVER_FILESYSTEM_HELPER_H
