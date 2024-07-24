#ifndef SM4_H
#define SM4_H
#include <stdint.h>
#define SM4_BLOCK_SIZE       (16)
struct sm4_ctx_t
{
        /*!<  SM4 subkeys       */
    uint32_t sk[32];

    /*initial vector */
    uint8_t iv[SM4_BLOCK_SIZE];
};
struct lyx_func_t
{
        void (*enckey)(sm4_ctx_t *ctx, uint8_t* key);
        void (*deckey)(sm4_ctx_t *ctx, uint8_t* key);
        int32_t (*encdata)(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output);
        int32_t (*decdata)(sm4_ctx_t *ctx, int32_t len, uint8_t *input, uint8_t *output);
};

enum SM4_TYPE
{
        SM4_MODE_ECB,
        SM4_MODE_CBC,
        SM4_MODE_CFB,
        SM4_MODE_OFB,
        SM4_MODE_CTR,
        SM4_MODE_MAX,
};
void sm4_func_register();
void sm4_func_unregister();
void lyx_sm4_register(lyx_func_t *op);
void lyx_sm4_unregister(void);

/**
 * desc		SM4 mode block encryption/decryption create
 * type		SM4 mode
 */
int32_t lyx_sm4_create(uint32_t type);
int32_t lyx_sm4_destroy(void);

/**
 * desc          SM4 key schedule (128-bit, encryption)
 *
 * param ctx      SM4 context to be initialized
 * param key      16-byte secret key
 */
int32_t lyx_sm4_enckey(sm4_ctx_t *ctx, uint8_t* key);

/**
 * desc          SM4 key schedule (128-bit, decryption)
 *
 * param ctx      SM4 context to be initialized
 * param key      16-byte secret key
 */
int32_t lyx_sm4_deckey(sm4_ctx_t *ctx, uint8_t* key);

/**
 * desc			SM4 block encryption/decryption
 * param ctx	SM4 context
 * param len	length of the input data
 * param input	input block
 * param output	output block
 */
int32_t lyx_sm4_encrypt(sm4_ctx_t *ctx, uint32_t len, uint8_t *input, uint8_t **output);
int32_t lyx_sm4_decrypt(sm4_ctx_t *ctx, uint32_t len, uint8_t *input, uint8_t **output);

#endif // SM4_H
