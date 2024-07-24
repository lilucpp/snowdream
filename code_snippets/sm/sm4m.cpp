#include "sm4m.h"

#include <cstring>
#include <iterator>
#include <string>

sm4m::sm4m(const char *key, const char *iv, unsigned int type) : mSm4Type(0) {
  sm4_func_register();
  mKey.reserve(16);
  mIv.reserve(16);
  memset(&mSm4EncCtx, 0, sizeof(mSm4EncCtx));
  memset(&mSm4DecCtx, 0, sizeof(mSm4DecCtx));
  mSm4Type = type;
  lyx_sm4_create(mSm4Type);
  setKeyAndIv(key, iv);
}

void sm4m::setKeyAndIv(const char *key, const char *iv) {
  if (key == nullptr) return;

  // mKey.reserve(16);
  std::copy(key, key + 16, std::back_inserter(mKey));

  lyx_sm4_enckey(&mSm4EncCtx, (unsigned char *)key);
  lyx_sm4_deckey(&mSm4DecCtx, (unsigned char *)key);

  if (iv) {
    memcpy(mSm4EncCtx.iv, iv, 16);
    memcpy(mSm4DecCtx.iv, iv, 16);
    std::copy(iv, iv + 16, std::back_inserter(mIv));
  }
}

void sm4m::setType(unsigned char type, void *key, void *iv) {
  mSm4Type = type;
  sm4_func_unregister();
  sm4_func_register();
  lyx_sm4_create(mSm4Type);
  setKeyAndIv((const char *)key, (const char *)iv);
}

unsigned char sm4m::getType() { return mSm4Type; }

int sm4m::encrypt(int32_t len, const uint8_t *input, uint8_t **output,
                  int32_t &outLen, bool isPadding) {
  if (input == nullptr || len == 0 || output == nullptr) return sm4_err_param;

  if (isPadding == false) {
    if (len % SM4_BLOCK_SIZE) return sm4_err_len;
    outLen = lyx_sm4_encrypt(&mSm4EncCtx, len, (unsigned char *)input, output);
    if (outLen <= -1) return sm4_err_encrypt;
    return sm4_ok;
  }

  uint8_t *iData = (uint8_t *)malloc(len + 16);
  if (iData == nullptr) return -1;

  memset(iData, 0, len + 16);
  int k = len % SM4_BLOCK_SIZE;
  unsigned char padding = SM4_BLOCK_SIZE - k;
  memcpy(iData, input, len);
  memset(iData + len, padding, padding);
  len += padding;
  outLen = lyx_sm4_encrypt(&mSm4EncCtx, len, (unsigned char *)iData, output);
  ::free(iData);
  if (outLen <= -1) return sm4_err_encrypt;

  return sm4_ok;
}

int sm4m::decrypt(int32_t len, const uint8_t *input, uint8_t **output,
                  int32_t &outLen, bool isPadding) {
  if (input == nullptr || len == 0 || output == nullptr) return sm4_err_param;

  if (len % SM4_BLOCK_SIZE) return sm4_err_len;

  outLen = lyx_sm4_decrypt(&mSm4DecCtx, len, (unsigned char *)input, output);
  if (outLen <= -1) return sm4_err_encrypt;
  uint8_t padding = (*output)[outLen - 1];
  if (isPadding) {
    if (padding > 16) return sm4_err_decrypt;
    outLen -= padding;
  }
  return sm4_ok;
}

void sm4m::releaseBuf(void *buf) {
  if (buf) free(buf);
  buf = nullptr;
}
