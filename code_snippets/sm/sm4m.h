#ifndef SM4M_H
#define SM4M_H

#include <vector>

#include "sm4.h"

class sm4m {
 public:
  /**
   * @brief sm4m
   * SM4，构造函数
   * @param key:待初始化的SM4 KEY密文，如果是空值，有内部产生
   * @param iv: 待初始化的SM4 IV密文，如果是空值，有内部产生
   * @param type: SM4算法类型，参考enum SM4_TYPE(sm4.h)
   */
  sm4m() {
    sm4_func_register();
    mKey.reserve(16);
    mIv.reserve(16);
  }
  sm4m(const char *key, const char *iv, unsigned int type = SM4_MODE_ECB);
  /**
   * @brief setKeyAndIv
   * 重新配置key 密文，IV密文
   * @param key: SM4 KEY的密文
   * @param iv: SM4 IV的密文
   */
  void setKeyAndIv(const char *key, const char *iv);
  /**
   * @brief setType
   * 重新配置SM4的算法类型
   * @param type: SM4算法类型，参考enum SM4_TYPE(sm4.h)
   * @param key:待初始化的SM4 KEY密文，如果是空值，有内部产生
   * @param iv: 待初始化的SM4 IV密文，如果是空值，有内部产生
   */
  void setType(unsigned char type, void *key, void *iv);
  unsigned char getType();
  std::vector<uint8_t> getKey() { return mKey; }
  std::vector<uint8_t> getIv() { return mIv; }
  /**
   * @brief encrypt
   * SM4加密
   * @param len:待加密数据长度
   * @param input:输入待加密数据
   * @param output:输出密文空间地址（需要releaseBuf释放）
   * @param outLen:输出密文长度
   * @param isPadding:是否有内部补齐(16字节的倍数)
   * @return 成功返回0，否则失败
   */
  int encrypt(int32_t len, const uint8_t *input, uint8_t **output,
              int32_t &outLen, bool isPadding = true);
  /**
   * @brief decrypt
   * SM4解密
   * @param len:待解密数据长度
   * @param input:输入待解密数据
   * @param output:输出明文空间地址(需要releaseBuf释放)
   * @param outLen:输出明文长度
   * @param isPadding:密文是否补齐过(16字节的倍数)
   * @return 成功返回0，否则失败
   */
  int decrypt(int32_t len, const uint8_t *input, uint8_t **output,
              int32_t &outLen, bool isPadding = true);
  /**
   * @brief releaseBuf
   * 释放空间
   * @param buf:待释放的空间
   */
  void releaseBuf(void *buf);

 public:
  enum {
    sm4_ok,
    sm4_err,
    sm4_err_key,
    sm4_err_param,
    sm4_err_len,
    sm4_err_encrypt,
    sm4_err_decrypt
  };

 protected:
  sm4_ctx_t mSm4EncCtx = {};
  sm4_ctx_t mSm4DecCtx = {};
  unsigned char mSm4Type = 0;
  std::vector<uint8_t> mKey = {};
  std::vector<uint8_t> mIv = {};
};

#endif  // SM4M_H
