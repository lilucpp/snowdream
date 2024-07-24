#include "sm4.h"
#include <stdlib.h>
#include<string.h>
#define SWAP(a,b) {a ^= b; b^= a; a ^= b;}
#define ROTL(x, n)	((x) << n | (x) >> (32 - n))
static const uint32_t sm4_fk[4] = {
    0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc,
};

static const uint32_t sm4_ck[32] = {
    0x00070e15,0x1c232a31,0x383f464d,0x545b6269,
    0x70777e85,0x8c939aa1,0xa8afb6bd,0xc4cbd2d9,
    0xe0e7eef5,0xfc030a11,0x181f262d,0x343b4249,
    0x50575e65,0x6c737a81,0x888f969d,0xa4abb2b9,
    0xc0c7ced5,0xdce3eaf1,0xf8ff060d,0x141b2229,
    0x30373e45,0x4c535a61,0x686f767d,0x848b9299,
    0xa0a7aeb5,0xbcc3cad1,0xd8dfe6ed,0xf4fb0209,
    0x10171e25,0x2c333a41,0x484f565d,0x646b7279
};

static const uint8_t sbox_table[256] = {
    0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05,
    0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99,
    0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62,
    0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6,
    0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8,
    0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35,
    0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87,
    0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e,
    0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1,
    0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3,
    0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f,
    0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51,
    0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8,
    0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0,
    0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84,
    0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48
};

static inline uint8_t sm4_sbox(uint8_t inch)
{
    return sbox_table[inch];
}

static inline uint32_t load_u32_be(const uint8_t *b)
{
    return b[0] << 24 | b[1] << 16 | b[2] << 8 | b[3];
}

static inline void store_u32_be(uint32_t v, uint8_t *b)
{
    b[0] = (uint8_t)(v >> 24);
    b[1] = (uint8_t)(v >> 16);
    b[2] = (uint8_t)(v >> 8);
    b[3] = (uint8_t)(v);
}

static uint32_t sm4_data_prep(uint32_t in)
{
    uint8_t a[4];
    uint8_t b[4];

    store_u32_be(in, a);
    b[0] = sm4_sbox(a[0]);
    b[1] = sm4_sbox(a[1]);
    b[2] = sm4_sbox(a[2]);
    b[3] = sm4_sbox(a[3]);

    return load_u32_be(b);
}

static uint32_t sm4_sk(uint32_t input)
{
    uint32_t key;

    key = sm4_data_prep(input);

    return key ^ ROTL(key, 13) ^ ROTL(key, 23);
}

static uint32_t sm4_sd(uint32_t input)
{
    uint32_t data;

    data = sm4_data_prep(input);

    return data ^ ROTL(data, 2) ^ ROTL(data, 10) ^ ROTL(data, 18) ^ ROTL(data, 24);
}

static void sm4_setkey(uint32_t* sk, uint8_t* key)
{
    uint32_t i;
    uint32_t k[4];
    uint32_t fk[36];

    k[0] = load_u32_be(key + 0);
    k[1] = load_u32_be(key + 4);
    k[2] = load_u32_be(key + 8);
    k[3] = load_u32_be(key + 12);

    fk[0] = k[0] ^ sm4_fk[0];
    fk[1] = k[1] ^ sm4_fk[1];
    fk[2] = k[2] ^ sm4_fk[2];
    fk[3] = k[3] ^ sm4_fk[3];

    for (i = 0; i < 32; i++) {
        fk[i + 4] = fk[i] ^ sm4_sk(fk[i + 1] ^ fk[i + 2] ^ fk[i + 3] ^ sm4_ck[i]);
        sk[i] = fk[i + 4];
    }
}

static void sm4_one_round(uint32_t* sk, uint8_t* input, uint8_t* output)
{
    uint32_t i;
    uint32_t ulbuf[36] = {0};

    ulbuf[0] = load_u32_be(input + 0);
    ulbuf[1] = load_u32_be(input + 4);
    ulbuf[2] = load_u32_be(input + 8);
    ulbuf[3] = load_u32_be(input + 12);

    for (i = 0; i < 32; i++) {
        ulbuf[i + 4] = ulbuf[i] ^ sm4_sd(ulbuf[i + 1] ^ ulbuf[i + 2] ^ ulbuf[i + 3] ^ sk[i]);
    }

    store_u32_be(ulbuf[35], output + 0);
    store_u32_be(ulbuf[34], output + 4);
    store_u32_be(ulbuf[33], output + 8);
    store_u32_be(ulbuf[32], output + 12);
}

static void sm4_encrypt_setkey(sm4_ctx_t *ctx, uint8_t* key)
{
    sm4_setkey(ctx->sk, key);
}

static void sm4_decrypt_setkey(sm4_ctx_t *ctx, uint8_t* key)
{
    int32_t i;

    sm4_setkey(ctx->sk, key);
    for ( i = 0; i < SM4_BLOCK_SIZE; i++) {
        SWAP(ctx->sk[i], ctx->sk[31-i]);
    }
}

static int32_t sm4_encrypt_ecb(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    uint32_t idx;
    uint8_t *p = input;

    for (idx = 0; idx < len; idx+= 16) {
        sm4_one_round(ctx->sk, p + idx, output + idx);
    }

    return idx;
}

static int32_t sm4_decrypt_ecb(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    int32_t idx;

    for (idx = 0; idx < len; idx+= 16) {
        sm4_one_round(ctx->sk, input + idx, output + idx);
    }

    return len;
}

static int32_t sm4_encrypt_cbc(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    int32_t i, idx;
    uint8_t* iv = ctx->iv;
    uint8_t temp[SM4_BLOCK_SIZE];

    for (idx = 0; idx < len; idx += SM4_BLOCK_SIZE) {
        for(i = 0; i < SM4_BLOCK_SIZE; i++) {
            temp[i] = input[i] ^ iv[i];
        }

        sm4_one_round(ctx->sk, temp, output);
        iv = output;

        input += SM4_BLOCK_SIZE;
        output += SM4_BLOCK_SIZE;
    }

    return idx;
}

static int32_t sm4_decrypt_cbc(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    int32_t i, idx;
    uint8_t* iv = ctx->iv;
    uint8_t temp[SM4_BLOCK_SIZE];

    for (idx = 0; idx < len; idx += SM4_BLOCK_SIZE) {
        sm4_one_round(ctx->sk, input, temp);

        for (i = 0; i < SM4_BLOCK_SIZE; i++) {
            output[i] = temp[i] ^ iv[i];
        }

        iv = input;
        input += SM4_BLOCK_SIZE;
        output += SM4_BLOCK_SIZE;
    }

    return len;
}

static int32_t sm4_encrypt_cfb(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    int32_t i, idx;
    uint8_t* iv = ctx->iv;
    uint8_t temp[SM4_BLOCK_SIZE];

    for (idx = 0; idx < len; idx += SM4_BLOCK_SIZE) {
        sm4_one_round(ctx->sk, iv, temp);

        for (i = 0; i < SM4_BLOCK_SIZE; i++) {
            output[i] = temp[i] ^ input[i];
        }

        iv = output;
        input += SM4_BLOCK_SIZE;
        output += SM4_BLOCK_SIZE;
    }

    return idx;
}

static int32_t sm4_decrypt_cfb(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    int32_t i, idx;
    uint8_t* iv = ctx->iv;
    uint8_t temp[SM4_BLOCK_SIZE];

    for (idx = 0; idx < len; idx += SM4_BLOCK_SIZE) {
        sm4_one_round(ctx->sk, iv, temp);

        for (i = 0; i < SM4_BLOCK_SIZE; i++) {
            output[i] = temp[i] ^ input[i];
        }

        iv = input;
        input += SM4_BLOCK_SIZE;
        output += SM4_BLOCK_SIZE;
    }

    return len;
}

static int32_t sm4_encrypt_ofb(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    int32_t i, idx;
    uint8_t* iv = ctx->iv;
    uint8_t temp[SM4_BLOCK_SIZE];

    for (idx = 0; idx < len; idx += SM4_BLOCK_SIZE) {
        sm4_one_round(ctx->sk, iv, temp);

        for (i = 0; i < SM4_BLOCK_SIZE; i++) {
            output[i] = temp[i] ^ input[i];
        }

        iv = temp;
        input += SM4_BLOCK_SIZE;
        output += SM4_BLOCK_SIZE;
    }

    return idx;
}

static int32_t sm4_decrypt_ofb(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    int32_t i, idx;
    uint8_t* iv = ctx->iv;
    uint8_t temp[SM4_BLOCK_SIZE];

    for (idx = 0; idx < len; idx += SM4_BLOCK_SIZE) {
        sm4_one_round(ctx->sk, iv, temp);

        for (i = 0; i < SM4_BLOCK_SIZE; i++) {
            output[i] = temp[i] ^ input[i];
        }

        iv = temp;
        input += SM4_BLOCK_SIZE;
        output += SM4_BLOCK_SIZE;
    }

    return len;
}

/* copy from openssl */
static void sm4_ctr_inc(unsigned char *counter)
{
    uint32_t n = SM4_BLOCK_SIZE;
    uint32_t c = 1;

    do {
        --n;
        c += counter[n];
        counter[n] = (uint8_t)c;
        c >>= 8;
    } while (n);
}

static int32_t sm4_encrypt_ctr(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    int32_t i, idx;
    uint8_t* iv = ctx->iv;
    uint8_t temp[SM4_BLOCK_SIZE];

    for (idx = 0; idx < len; idx += SM4_BLOCK_SIZE) {
        sm4_one_round(ctx->sk, iv, temp);

        for (i = 0; i < SM4_BLOCK_SIZE; i++) {
            output[i] = temp[i] ^ input[i];
        }

        sm4_ctr_inc(iv);
        input += SM4_BLOCK_SIZE;
        output += SM4_BLOCK_SIZE;
    }

    return idx;
}

static int32_t sm4_decrypt_ctr(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output)
{
    return sm4_encrypt_ctr(ctx, len, input, output);
}

static lyx_func_t sm4_func[SM4_MODE_MAX] = {
    {sm4_encrypt_setkey, sm4_decrypt_setkey, sm4_encrypt_ecb, sm4_decrypt_ecb,},
    {sm4_encrypt_setkey, sm4_decrypt_setkey, sm4_encrypt_cbc, sm4_decrypt_cbc,},
    {sm4_encrypt_setkey, sm4_encrypt_setkey, sm4_encrypt_cfb, sm4_decrypt_cfb,},
    {sm4_encrypt_setkey, sm4_encrypt_setkey, sm4_encrypt_ofb, sm4_decrypt_ofb,},
    {sm4_encrypt_setkey, sm4_encrypt_setkey, sm4_encrypt_ctr, sm4_decrypt_ctr,},
};

void sm4_func_register()
{
    lyx_sm4_register(sm4_func);
}
void sm4_func_unregister()
{
    lyx_sm4_unregister();
}


static lyx_func_t *ops;
void lyx_sm4_register(lyx_func_t *op)
{
    ops = op;
}

void lyx_sm4_unregister(void)
{
    ops = nullptr;
}

int32_t lyx_sm4_create(uint32_t type)
{
    lyx_func_t *func = nullptr;

    if (nullptr == ops || type >= SM4_MODE_MAX) {
        return -1;
    }

    func = ops;

    ops = &func[type];

    return 0;
}

int32_t lyx_sm4_destroy(void)
{
    return 0;
}

int32_t lyx_sm4_enckey(sm4_ctx_t *ctx, uint8_t* key)
{
    ops->enckey(ctx, key);

    return 0;
}

int32_t lyx_sm4_deckey(sm4_ctx_t *ctx, uint8_t* key)
{
    ops->deckey(ctx, key);
    return 0;
}
#if 1
int32_t lyx_sm4_encrypt(sm4_ctx_t *ctx, uint32_t len, uint8_t *input, uint8_t **output)
{
    if(len % SM4_BLOCK_SIZE)
        return -1;
    *output = (uint8_t*)malloc(len);
    if(*output == nullptr)
        return -1;
    memset(*output,0,len);
    int32_t ret = ops->encdata(ctx, len, input, *output);
    return ret;

}

int32_t lyx_sm4_decrypt(sm4_ctx_t *ctx, uint32_t len, uint8_t *input, uint8_t **output)
{
    *output = (uint8_t*)malloc(len+16);
    memset(*output,0,len+16);
    int32_t ret = ops->decdata(ctx, len, input, *output);
    return ret;
}
#endif
void freeBuf(uint8_t *output)
{
    if(output)
        free(output);
    output = nullptr;
}
