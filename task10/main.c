#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <openssl/des.h>

#define ROUNDS 25
#define SALT_LENGTH 2

void generate_salt(unsigned char *salt) {
    for (int i = 0; i < SALT_LENGTH; i++) {
        salt[i] = rand() & 0xFF;
    }
}

void encode_byte(unsigned char byte, char *out) {
    out[0] = 'a' + ((byte >> 4) & 0x0F);
    out[1] = 'a' + (byte & 0x0F);
}

unsigned char decode_byte(const char *in) {
    return ((in[0] - 'a') << 4) | (in[1] - 'a');
}

void encrypt_password(const char *password, const unsigned char *salt, char *encrypted) {
    DES_cblock key, temp;
    DES_key_schedule schedule;

    memset(key, 0, sizeof(DES_cblock));
    memcpy(key, salt, SALT_LENGTH);
    strncpy((char *)key + SALT_LENGTH, password, sizeof(DES_cblock) - SALT_LENGTH);

    DES_set_odd_parity(&key);
    if (DES_set_key_checked(&key, &schedule) != 0) {
        fprintf(stderr, "Weak key generated\n");
        exit(1);
    }

    memset(temp, 0, sizeof(DES_cblock));
    for (int i = 0; i < ROUNDS; i++) {
        DES_ecb_encrypt(&temp, &temp, &schedule, DES_ENCRYPT);
    }

    encode_byte(salt[0], &encrypted[0]);
    encode_byte(salt[1], &encrypted[2]);

    for (int i = 0; i < sizeof(DES_cblock); i++) {
        encode_byte(temp[i], &encrypted[4 + i * 2]);
    }

    encrypted[4 + 2 * sizeof(DES_cblock)] = '\0';
}

int verify_password(const char *password, const char *encrypted_with_salt) {
    unsigned char salt[SALT_LENGTH];
    salt[0] = decode_byte(&encrypted_with_salt[0]);
    salt[1] = decode_byte(&encrypted_with_salt[2]);

    char encrypted[4 + 2 * sizeof(DES_cblock) + 1];
    encrypt_password(password, salt, encrypted);

    return strcmp(encrypted_with_salt, encrypted) == 0;
}

int main() {
    srand(time(NULL));

    printf("Generating 10 encrypted passwords:\n");
    for (int i = 0; i < 10; i++) {
        char password[32];
        unsigned char salt[SALT_LENGTH];
        char encrypted[4 + 2 * sizeof(DES_cblock) + 1];

        snprintf(password, sizeof(password), "pass%d", i);

        generate_salt(salt);
        encrypt_password(password, salt, encrypted);

        printf("Password: %-8s Encrypted: %s\n", password, encrypted);
    }

    printf("\nPassword verification test:\n");
    const char *test_password = "abdelrahman is the best programmer";
    unsigned char test_salt[SALT_LENGTH];
    char test_encrypted[4 + 2 * sizeof(DES_cblock) + 1];

    generate_salt(test_salt);
    encrypt_password(test_password, test_salt, test_encrypted);

    printf("Encrypted '%s' as: %s\n", test_password, test_encrypted);
    printf("Verifying with correct password: %s\n",
           verify_password(test_password, test_encrypted) ? "SUCCESS" : "FAILURE");
    printf("Verifying with wrong password: %s\n",
           verify_password("wrongpass", test_encrypted) ? "SUCCESS" : "FAILURE");

    return 0;
}

