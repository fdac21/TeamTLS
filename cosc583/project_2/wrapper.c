#include "openssl/sha.h"
#include "stdio.h"

typedef unsigned char uint8;

int main(int argc, char const *argv[])
{
    SHA256_CTX sha256;
    int test = SHA256_Init(&sha256);
    printf("Testing OpenSSL SHA256: %d\n", test);
    return 0;
}
