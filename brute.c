#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char *plain = (unsigned char *)"This is a top secret.";
char *cipher = "764aa26b55a4da654df6b19e4bce00f4ed05e09346fb0e762583cb7da2ac93a2";
unsigned char iv[16];

void wordlength(char *str)
{
    int len = strlen(str);
    while (len < 16)
    {
        str[len] = '#';
        len++;
    }
    str[16] = '\0';
}

int do_crypt(unsigned char *input, int input_len, unsigned char *key, unsigned char *iv, unsigned char *output)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        return 0;

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        return 0;

    if (1 != EVP_EncryptUpdate(ctx, output, &len, input, input_len))
        return 0;
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, output + len, &len))
        return 0;
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

void hex_to_bytes(const char *hex, unsigned char *bytes, int len)
{
    for (int i = 0; i < len; i++)
    {
        sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
    }
}

int main()
{
    FILE *wordlist;
    if ((wordlist = fopen("/home/seed/Desktop/Labsetup/words.txt", "r")) == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    hex_to_bytes("aabbccddeeff00998877665544332211", iv, 16);

    char word[17];
    unsigned char out_cipher[1024];
    char out_cipher_hex[1024 * 2 + 1];
    unsigned char target_cipher[32];

    hex_to_bytes(cipher, target_cipher, 32);

    while (fgets(word, sizeof(word), wordlist) != NULL)
    {
        char *newline = strchr(word, '\n');
        if (newline)
            *newline = '\0';

        wordlength(word);
        int outlen = do_crypt(plain, strlen((char *)plain), (unsigned char *)word, iv, out_cipher);

        if (outlen == 32 && memcmp(out_cipher, target_cipher, 32) == 0)
        {
            printf("Key Found: %s\n", word);
            break;
        }
    }

    fclose(wordlist);
    return 0;
}