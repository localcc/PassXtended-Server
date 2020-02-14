//
// Created by localcc on 2/14/20.
//
#include <tls_helper/tls_helper.h>
#include <config.h>
#include <filesystem>
#include <cstdio>
#include <iostream>

EVP_PKEY* tls_helper::gen_key() {
    EVP_PKEY* pkey = EVP_PKEY_new();
    if(!pkey) return nullptr;
    RSA* rsa = RSA_new();
    BIGNUM* bn = BN_new();
    BN_set_word(bn, RSA_F4);
    
    RSA_generate_key_ex(rsa, 4096, bn, nullptr);
    if(!EVP_PKEY_assign_RSA(pkey, rsa)) {
        EVP_PKEY_free(pkey);
        return nullptr;
    }
    return pkey;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
X509* tls_helper::gen_x509(EVP_PKEY* pkey) {
    X509* x509 = X509_new();
    if(!x509) return nullptr;

    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

    //TODO: choose amount of time certificate is valid
    //1 year
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);

    X509_set_pubkey(x509, pkey);
    X509_NAME* name = X509_get_subject_name(x509);

    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, CERT_C, -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, CERT_O, -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, CERT_CN, -1, -1, 0);

    X509_set_issuer_name(x509, name);
    if(!X509_sign(x509, pkey, EVP_sha1())) {
        X509_free(x509);
        //TODO: error output
        return nullptr;
    }
    return x509;
}
#pragma clang diagnostic pop

bool tls_helper::write_to_disk(X509* cert, EVP_PKEY* key) {
    FILE* pkey_file;

    pkey_file = fopen((storage_folder / "key.pem").c_str(), "wb");
    if(!pkey_file) return false;

    bool ret = PEM_write_PrivateKey(pkey_file, key, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(pkey_file);
    if(!ret) return false;

    FILE* x509_file;
    x509_file = fopen((storage_folder / "cert.pem").c_str(), "wb");
    if(!x509_file) return false;

    ret = PEM_write_X509(x509_file, cert);
    fclose(x509_file);
    return ret;
}

void tls_helper::init_tls() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();

}

SSL_CTX* tls_helper::gen_context() {
    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    return ctx;
}

void tls_helper::config_ssl(SSL* ssl) {
    SSL_set_cipher_list(ssl, "TLS_ECDHE_RSA_AES256_GCM_SHA384");
}

void tls_helper::config_ctx(SSL_CTX* ctx) {
    SSL_CTX_set_ecdh_auto(ctx, 1);

    if(SSL_CTX_use_certificate_file(ctx, (storage_folder / "cert.pem").c_str(), SSL_FILETYPE_PEM) <= 0) {
        //TODO: error printing
        std::cout << "Failed to load cert\n";
        return;
    }
    if(SSL_CTX_use_PrivateKey_file(ctx, (storage_folder / "key.pem").c_str(), SSL_FILETYPE_PEM) <= 0) {
        //TODO: error printing
        std::cout << "Failed to load private key\n";
    }
}