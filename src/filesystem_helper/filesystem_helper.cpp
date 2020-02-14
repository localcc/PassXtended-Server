//
// Created by localcc on 2/5/20.
//

#include <filesystem_helper/filesystem_helper.h>
#include <fstream>
#include <iostream>

filesystem_helper* filesystem_helper::current_instance = nullptr;

filesystem_helper::filesystem_helper(std::string dir_path) : dir_path(dir_path) {
    current_instance = this;
}

//                      filename    data   size
std::vector<std::tuple<std::string, int8_t*, int>> filesystem_helper::read_all_files() {

    std::vector<std::tuple<std::string, int8_t*, int>> files;
    std::filesystem::recursive_directory_iterator iterator(this->dir_path);
    for(const auto& path : iterator) {
        if(path.is_directory()) continue;
        std::ifstream fin(path.path(), std::ios::binary);
        //TODO: error printing
        if(!fin) std::cout << "Failed to open fin!\n";



        fin.seekg(0, std::ifstream::end);
        int length = fin.tellg();
        fin.seekg(0, std::ifstream::beg);

        auto* data = new int8_t[length];
        if(!fin.read(reinterpret_cast<char *>(data), length)) {
            //TODO: error printing
            std::cout << "Failed to read!\n";
        }
        files.emplace_back(path.path(), data, length);
    }
    return files;
}

void filesystem_helper::write_file(const std::string& file_name, const char* data, int size) {
    std::ofstream fout(file_name, std::ios::binary);
    fout.write(data, size);
    fout.close();
}


filesystem_helper::~filesystem_helper() {
    this->dir_path.clear();

}
