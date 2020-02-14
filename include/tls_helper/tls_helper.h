//
// Created by localcc on 2/14/20.
//

#ifndef PASSXTENDED_SERVER_TLS_HELPER_H
#define PASSXTENDED_SERVER_TLS_HELPER_H

#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

class tls_helper {
public:
    static EVP_PKEY* gen_key();
    static X509* gen_x509(EVP_PKEY* pkey);
    static bool write_to_disk(X509* cert, EVP_PKEY* key);
    static void init_tls();
    static SSL_CTX* gen_context();
    static void config_ssl(SSL* ssl);
    static void config_ctx(SSL_CTX* ctx);
};


#endif //PASSXTENDED_SERVER_TLS_HELPER_H
