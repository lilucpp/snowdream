#include "example_sms4.h"
#include "utility.h"
#include <openssl/sms4.h>
#include <openssl/evp.h>
#include <cstring>
int evp_sms4()
{
	sms4_key_t key;
	const int TEXT_SIZE = 17;
	unsigned char enBuf[TEXT_SIZE] = { 0 };
	unsigned char deBuf[TEXT_SIZE] = { 0 };
	unsigned char user_key[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };//秘钥
	unsigned char plaintext[TEXT_SIZE] = "abcdefghijklmnop";
	//明文
	printf("plain text:");
	print_text(plaintext, sizeof(plaintext));

	//加密
	sms4_set_encrypt_key(&key, user_key);
	sms4_encrypt(plaintext, enBuf, &key);
	printf("cipher text:");
	print_text(enBuf, sizeof(enBuf));

	//解密
	sms4_set_decrypt_key(&key, user_key);
	sms4_decrypt(enBuf, deBuf, &key);
	printf("decrypt text:");
	print_text(deBuf, sizeof(deBuf));

	return 1;
}

int evp_sms4_cbc()
{
	/* Bogus key and IV: we'd normally set these from another source.*/
	unsigned char key[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	unsigned char iv[] = { 1,2,3,4,5,6,7,8 };
	unsigned char intext[] = "Some Crypto Text";
	unsigned char *enBuf = (unsigned char *)malloc(strlen((char*)intext)+ EVP_MAX_BLOCK_LENGTH);
	unsigned char *deBuf = (unsigned char *)malloc(strlen((char*)intext)+ EVP_MAX_BLOCK_LENGTH);

	int enLen, enTmplen;
	int deLen, deTmplen;
	printf("plain text:");
	print_text(intext, strlen((char*)intext));

	//加密
	EVP_CIPHER_CTX *ctx= EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);
	EVP_EncryptInit_ex(ctx, EVP_sms4_cbc(), NULL, key, iv);
	if (!EVP_EncryptUpdate(ctx, enBuf, &enLen, intext, strlen((char*)intext))) {
		return 0;
	}
	if (!EVP_EncryptFinal_ex(ctx, enBuf + enLen, &enTmplen)) {
		return 0;
	}
	enLen += enTmplen;
	printf("cipher text:");
	print_text(enBuf, enLen);

	//解密
	unsigned char iv2[] = { 0,2,3,4,5,6,7,8 };
	EVP_DecryptInit_ex(ctx, EVP_sms4_cbc(), NULL, key, iv);
	if (!EVP_DecryptUpdate(ctx, deBuf, &deLen, enBuf, enLen)) {
		return 0;
	}
	if (!EVP_DecryptFinal_ex(ctx, deBuf, &deTmplen)) {
		return 0;
	}
	deLen += deTmplen;
	printf("decrypt text:");
	print_text(deBuf, deLen);

	//释放资源
	free(enBuf);
	free(deBuf);
	EVP_CIPHER_CTX_cleanup(ctx);
	EVP_CIPHER_CTX_free(ctx);

	return 1;
}
