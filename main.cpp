#include <iostream>
#include <tcp_server/tcp_server.h>
#include <filesystem_helper/filesystem_helper.h>
#include <tls_helper/tls_helper.h>
#include <filesystem>
#include <config.h>

int main() {
    if(!std::filesystem::exists(storage_folder)) std::filesystem::create_directory(storage_folder);
    if(!std::filesystem::exists(storage_folder / "cert.pem") || !std::filesystem::exists(storage_folder / "key.pem")) {
        std::cout << "Generating new cert/key pair\n";
        EVP_PKEY* pkey = tls_helper::gen_key();
        X509* x509 = tls_helper::gen_x509(pkey);
        tls_helper::write_to_disk(x509, pkey);
    }

    filesystem_helper* helper = new filesystem_helper(folder_path);
    tcp_server *s = new tcp_server(1135);
    s->start();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
