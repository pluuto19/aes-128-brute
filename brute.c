#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char *plain = (unsigned char *)"This is a top secret.";
char *cipher = "764aa26b55a4da654df6b19e4bce00f4ed05e09346fb0e762583cb7da2ac93a2";
const unsigned char *iv = "aabbccddeeff00998877665544332211";

void wordlength(char *str)
{
    int len = strlen(str);
    if (len == 16)
    {
        return;
    }
    while (len < 16)
    {
        str[len] = '#';
        len++;
    }
}

int do_crypt(unsigned char *input, int input_len, unsigned char *key, unsigned char *output, int do_encrypt)
{
    unsigned char outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int outlen, tmplen;
    EVP_CIPHER_CTX *ctx;

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        return 0;
    }

    if (!EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, do_encrypt))
    {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
    OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);

    if (!EVP_CipherInit_ex(ctx, NULL, NULL, key, iv, do_encrypt))
    {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    if (!EVP_CipherUpdate(ctx, outbuf, &outlen, input, input_len))
    {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    if (!EVP_CipherFinal_ex(ctx, outbuf + outlen, &tmplen))
    {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    outlen += tmplen;
    memcpy(output, outbuf, outlen);
    EVP_CIPHER_CTX_free(ctx);
    return outlen;
}

void bin_to_hex(const unsigned char *bin, int len, char *hex)
{
    for (int i = 0; i < len; i++)
    {
        sprintf(hex + (i * 2), "%02x", bin[i]);
    }
    hex[len * 2] = '\0';
}

int main()
{
    FILE *wordlist;
    if ((wordlist = fopen("/home/seed/Desktop/Labsetup/words.txt", "r")) == NULL)
    {
        printf("Error opening file");
        return 1;
    }

    char word[16];
    unsigned char out_cipher[1024];
    char out_cipher_hex[1024 * 2 + 1];

    while (fgets(word, sizeof(word), wordlist) != NULL)
    {
        char *newline = strchr(word, '\n');
        if (newline)
        {
            *newline = '\0';
        }
        wordlength(word);
        int outlen = do_crypt(plain, strlen((char *)plain), (unsigned char *)word, out_cipher, 1);
        bin_to_hex(out_cipher, outlen, out_cipher_hex);
        if(strcmp(cipher, out_cipher_hex) == 0){
            printf("Key Found %s\n", word);
            break;
        }
    }

    fclose(wordlist);
    return 0;
}
