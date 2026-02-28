#pragma once
#include "logger.hpp"

#include "aes.hpp"
#include "mbedtls/aes.h"

#define TEST_SIZE 1024
#define AES_BLOCK_SIZE 16

size_t get_pkcs7_padded_len(size_t len)
{
    return (len / 16 + 1) * 16;
}

void add_pkcs7_padding(uint8_t *buf, size_t data_len, size_t padded_len)
{
    uint8_t padding_val = padded_len - data_len;
    for (size_t i = data_len; i < padded_len; i++)
    {
        buf[i] = padding_val;
    }
}

size_t strip_pkcs7_padding(uint8_t *buf, size_t padded_len)
{
    uint8_t padding_val = buf[padded_len - 1];
    if (padding_val > 16 || padding_val == 0)
        return 0; 
    return padded_len - padding_val;
}

void taskAES(void *parameters)
{
    int core = xPortGetCoreID();
    const char *original_text = "电机控制指令: Zielort 5000 Schritte, Рабочая частота 5 кГц, Hardware emergency stop is ready,Secuencia de verificación de seguridad:ESP32-AES-TEST-99k#";
    size_t text_len = strlen(original_text);
    size_t padded_len = get_pkcs7_padded_len(text_len);
    uint8_t key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint8_t iv[16]; // CBC Mode
    uint8_t input[TEST_SIZE];
    uint8_t output_sw[TEST_SIZE];
    uint8_t output_hw[TEST_SIZE];
    for (;;)
    {
        uint8_t *plain_buf = (uint8_t *)heap_caps_malloc(padded_len, MALLOC_CAP_8BIT);
        uint8_t *cipher_buf = (uint8_t *)heap_caps_malloc(padded_len, MALLOC_CAP_8BIT);
        uint8_t *decrypt_buf = (uint8_t *)heap_caps_malloc(padded_len, MALLOC_CAP_8BIT);

        memcpy(plain_buf, original_text, text_len);
        add_pkcs7_padding(plain_buf, text_len, padded_len);

        mbedtls_aes_context aes;
        mbedtls_aes_init(&aes);

        // ==========================================
        // 1. ECB Mode Test
        // ==========================================
        mbedtls_aes_setkey_enc(&aes, key, 128);
        for (int i = 0; i < padded_len; i += 16)
        {
            mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, plain_buf + i, cipher_buf + i);
        }

        mbedtls_aes_setkey_dec(&aes, key, 128);
        for (int i = 0; i < padded_len; i += 16)
        {
            mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, cipher_buf + i, decrypt_buf + i);
        }

        size_t final_len = strip_pkcs7_padding(decrypt_buf, padded_len);
        decrypt_buf[final_len] = '\0';
        logD("ECB decrypt verify: %s", strcmp((char *)decrypt_buf, original_text) == 0 ? "✅ Succeed" : "❌ Failed");

        // ==========================================
        // 2. CBC Mode Test
        // ==========================================
        memset(iv, 0x00, 16);
        mbedtls_aes_setkey_enc(&aes, key, 128);
        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, padded_len, iv, plain_buf, cipher_buf);

        memset(iv, 0x00, 16);
        mbedtls_aes_setkey_dec(&aes, key, 128);
        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, padded_len, iv, cipher_buf, decrypt_buf);

        final_len = strip_pkcs7_padding(decrypt_buf, padded_len);
        decrypt_buf[final_len] = '\0';
        logD("CBC decrypt verify: %s", strcmp((char *)decrypt_buf, original_text) == 0 ? "✅ Succeed" : "❌ Failed");
        mbedtls_aes_free(&aes);
        free(plain_buf);
        free(cipher_buf);
        free(decrypt_buf);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}