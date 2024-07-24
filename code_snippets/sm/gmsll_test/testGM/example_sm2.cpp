#include "example_sm2.h"
#include <openssl/sm2.h>
#include <openssl/ec.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include "utility.h"
#include <string>
#include <sstream>


EVP_PKEY *genpkey(int curve_nid)
{
	int ok = 0;
	EVP_PKEY *ret = NULL;
	EVP_PKEY_CTX *pkctx = NULL;

	if (!(pkctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL))) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_keygen_init(pkctx)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pkctx, curve_nid)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_keygen(pkctx, &ret)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	ok = 1;
end:
	if (!ok && ret) {
		EVP_PKEY_free(ret);
		ret = NULL;
	}
	EVP_PKEY_CTX_free(pkctx);
	return ret;
}

int evp_pkey_encrypt_decrypt(EVP_PKEY *pkey)
{
	int ret = 0;
	EVP_PKEY_CTX *pkctx = NULL;
	int type = NID_sm_scheme;
	unsigned char msg[] = "hello world this is the message";
	size_t msglen = sizeof(msg);
	printf("plaintext:");
	print_text(msg, msglen);
	unsigned char cbuf[512];
	size_t cbuflen = sizeof(cbuf);
	unsigned char mbuf[512];
	size_t mbuflen = sizeof(mbuf);

	if (!(pkctx = EVP_PKEY_CTX_new(pkey, NULL))) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_encrypt_init(pkctx)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_CTX_set_ec_enc_type(pkctx, type)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	//加密
	cbuflen = sizeof(cbuf);
	if (!EVP_PKEY_encrypt(pkctx, cbuf, &cbuflen, msg, msglen)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}
	printf("cipher text:");
	print_text(cbuf, cbuflen);

	if (!EVP_PKEY_decrypt_init(pkctx)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_CTX_set_ec_enc_type(pkctx, type)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	memset(mbuf, 0, sizeof(mbuf));
	mbuflen = sizeof(mbuf);
	//解密
	if (!EVP_PKEY_decrypt(pkctx, mbuf, &mbuflen, cbuf, cbuflen)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}
	printf("decrypt text:");
	print_text(mbuf, mbuflen);

	ret = 1;
end:
	EVP_PKEY_CTX_free(pkctx);
	return ret;
}

int evp_pkey_sign(EVP_PKEY *pkey)
{
	int ret = 0;
	EVP_PKEY_CTX *pkctx = NULL;
	int type = NID_sm_scheme;
	unsigned char dgst[EVP_MAX_MD_SIZE] = "hello world";
	size_t dgstlen;
	unsigned char sig[256];
	size_t siglen;

	if (!(pkctx = EVP_PKEY_CTX_new(pkey, NULL))) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_sign_init(pkctx)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_CTX_set_ec_sign_type(pkctx, type)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	dgstlen = 32;
	memset(sig, 0, sizeof(sig));
	siglen = sizeof(sig);
	//签名
	if (!EVP_PKEY_sign(pkctx, sig, &siglen, dgst, dgstlen)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}
	printf("sign:");
	print_text(sig, siglen);

	if (!EVP_PKEY_verify_init(pkctx)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_CTX_set_ec_sign_type(pkctx, type)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}
	//核实
	if (1 != EVP_PKEY_verify(pkctx, sig, siglen, dgst, dgstlen)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	ret = 1;
end:
	EVP_PKEY_CTX_free(pkctx);
	return ret;
}


int evp_sign(EVP_PKEY *pkey, const EVP_MD *md)
{
	int ret = 0;
	EVP_MD_CTX *mdctx = NULL;
	unsigned char msg[] = "hello world this is the message";
	size_t msglen = sizeof(msg);
	unsigned char sig[256];
	unsigned int siglen = (unsigned int)sizeof(sig);

	if (!(mdctx = EVP_MD_CTX_create())) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_SignInit_ex(mdctx, md, NULL)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_SignUpdate(mdctx, msg, msglen)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_SignFinal(mdctx, sig, &siglen, pkey)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	printf("sign:");
	print_text(sig, siglen);

	if (!EVP_VerifyInit_ex(mdctx, md, NULL)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_VerifyUpdate(mdctx, msg, msglen)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (1 != EVP_VerifyFinal(mdctx, sig, siglen, pkey)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	ret = 1;
end:
	EVP_MD_CTX_destroy(mdctx);
	return ret;
}

void evp_ec()
{
	//生成密钥
	EVP_PKEY *pkey = genpkey(NID_sm2p256v1);
	print_key(EVP_PKEY_get1_EC_KEY(pkey));

	//加密 解密
	if (!evp_pkey_encrypt_decrypt(pkey)) {
		return ;
	}

	//签名
	if (!evp_pkey_sign(pkey)) {
		return ;
	}

	//消息摘要签名
	const EVP_MD *md = EVP_sm3();
	if (!evp_sign(pkey, md)) {
		return ;
	}

	//释放资源
	if (pkey != NULL) {
		EVP_PKEY_free(pkey);
	}
}

#include <stdio.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <cassert>

int error_callback(const char* str, size_t len, void* u) {
	fprintf(stderr, "OpenSSL Error: %s\n", str);
	return 0;
}

void save_ECPrivateKey_to_file(const char* filename, EC_KEY* ec_key) {
	// 设置错误处理回调函数
	ERR_print_errors_cb(error_callback, NULL);

	FILE* fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("Error opening file for writing: %s\n", filename);
		return;
	}

	if(EC_KEY_print_fp(fp, ec_key, 1) > 0) {
		printf("success save\n");
	}

	/*
	EVP_PKEY* evp_key = EVP_PKEY_new();
	if (!EVP_PKEY_set1_EC_KEY(evp_key, ec_key)) {
		printf("Error setting EC key\n");
		fclose(fp);
		return;
	}

	// log
	printf("-----------------\n");
	print_key(EVP_PKEY_get1_EC_KEY(evp_key));
	printf("-----------------\n");
	//void print_key(EC_KEY * ec_key)
	//{
		BIO* out = NULL;
		out = BIO_new_fp(stderr, BIO_NOCLOSE);
		EC_KEY_print(out, ec_key, 4);
		BIO_printf(out, "\n");
		BIO_free(out);
	//}




	BIO* bio = BIO_new_fp(fp, BIO_CLOSE);
	if (!PEM_write_bio_PrivateKey(bio, evp_key, NULL, NULL, 0, NULL, NULL)) {
	
		fclose(fp);
		printf("Error writing EC private key to file: %s\n", filename);
	}

	BIO_free(bio);
	EVP_PKEY_free(evp_key);*/
	fclose(fp);
}



void write_ECPrivateKey_to_file(const char* filename, EC_KEY* ec_key) {
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		fprintf(stderr, "Error opening file for writing: %s\n", filename);
		return;
	}

	if (!PEM_write_ECPrivateKey(fp, ec_key, NULL, NULL, 0, NULL, NULL)) {
		fprintf(stderr, "Error writing EC private key to file: %s\n", filename);
	}

	fclose(fp);
}

void write_ECPublicKey_to_file(const char* filename, EC_KEY* ec_key) {
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		fprintf(stderr, "Error opening file for writing: %s\n", filename);
		return;
	}

	if (!PEM_write_EC_PUBKEY(fp, ec_key)) {
		fprintf(stderr, "Error writing EC public key to file: %s\n", filename);
	}

	fclose(fp);
}

void createsm2() {

	//1.生成密钥
	EC_KEY* sm2key = EC_KEY_new_by_curve_name(NID_sm2p256v1);
	EC_KEY_generate_key(sm2key);
	print_key(sm2key);

	assert(sm2key != NULL);

	EC_KEY* privateKey = EC_KEY_new();
	EC_KEY* publicKey = EC_KEY_new();
	const EC_GROUP* ec_group = EC_KEY_get0_group(sm2key);

	//2.私钥
	EC_KEY_set_group(privateKey, ec_group);
	EC_KEY_set_private_key(privateKey, EC_KEY_get0_private_key(sm2key));
	print_key(privateKey);

	//3.公钥
	EC_KEY_set_group(publicKey, ec_group);
	EC_KEY_set_public_key(publicKey, EC_KEY_get0_public_key(sm2key));
	print_key(publicKey);

	// 将私钥、公钥保存到文件
	// save_ECPrivateKey_to_file("private_key.pem", privateKey);
	//save_ECPrivateKey_to_file("public_key.pem", publicKey);

	// 私钥写入为空， 公钥可以写入文件
	// 私钥 写入有问题。---------
	// write_ECPrivateKey_to_file("private_key.pem", privateKey);
	write_ECPublicKey_to_file("public_key.pem", publicKey);


	//释放资源
	EC_KEY_free(sm2key);
	EC_KEY_free(privateKey);
	EC_KEY_free(publicKey);

}

EC_KEY* read_ECPrivateKey_from_file(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error opening file for reading: %s\n", filename);
		return NULL;
	}

	EC_KEY* ec_key = NULL;
	ec_key = PEM_read_ECPrivateKey(fp, &ec_key, NULL, NULL);
	if (ec_key == NULL) {
		printf("Error reading EC private key from file: %s\n", filename);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	return ec_key;
}

EC_KEY* read_ECPublicKey_from_file(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error opening file for reading: %s\n", filename);
		return NULL;
	}

	EC_KEY* ec_key = PEM_read_EC_PUBKEY(fp, NULL, NULL, NULL);
	if (ec_key == NULL) {
		printf("Error reading EC public key from file: %s\n", filename);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	return ec_key;
}

void test_readkeyfromfile() {
	EC_KEY* private_key = read_ECPrivateKey_from_file("private_key.pem");
	EC_KEY* public_key = read_ECPublicKey_from_file("public_key.pem");

	print_key(private_key);
	print_key(public_key);

	EC_KEY_free(private_key);
	EC_KEY_free(public_key);

}

void hexStringToUnsignedCharArray(const std::string& hexString, char out[256]) {
	size_t length = hexString.length() / 2;
	for (size_t i = 0; i < length; ++i) {
		std::string byteString = hexString.substr(i * 2, 2);
		unsigned int byteValue;
		std::istringstream(byteString) >> std::hex >> byteValue;
		out[i] = static_cast<unsigned char>(byteValue);
	}
}

void testVrify() {
	EC_KEY* public_key = read_ECPublicKey_from_file("public_key.pem");

	unsigned char sig[256];
	unsigned int siglen;
	siglen = sizeof(sig);
	int type = NID_sm_scheme;
	unsigned char dgst[EVP_MAX_MD_SIZE] = "hello123";
	size_t dgstlen = strlen((char*)dgst);

	FILE* fp = fopen("demo.sig", "r");
	size_t r1 = fread(sig, sizeof sig[0], siglen, fp);
	fclose(fp);

	//7.核实
	if (!SM2_verify(type, dgst, dgstlen, sig, siglen, public_key)) {
		printf("verify failed\n");
	}

	printf("verify success\n");

	EC_KEY_free(public_key);
}


void testSignVrify() {
	EC_KEY* private_key = read_ECPrivateKey_from_file("private_key.pem");
	EC_KEY* public_key = read_ECPublicKey_from_file("public_key.pem");

	print_key(private_key);
	print_key(public_key);

	unsigned char sig[256];
	unsigned int siglen;
	siglen = sizeof(sig);
	int type = NID_sm_scheme;
	unsigned char dgst[EVP_MAX_MD_SIZE] = "hello123";
	size_t dgstlen = strlen((char*)dgst);
	printf("dgst text:");
	print_text(dgst, dgstlen);

	//6.签名
	SM2_sign(type, dgst, dgstlen, sig, &siglen, private_key);
	printf("sign:");
	print_text(sig, siglen);
	FILE* fp = fopen("demo.sig", "wb");
	size_t r1 = fwrite(sig, sizeof sig[0], siglen, fp);
	fclose(fp);

	//7.核实
	if (!SM2_verify(type, dgst, dgstlen, sig, siglen, public_key)) {
		printf("verify failed\n");
	}
	printf("verify success\n");

	EC_KEY_free(private_key);
	EC_KEY_free(public_key);
}

void evp_sm2()
{
	//1.生成密钥
	EC_KEY *sm2key = EC_KEY_new_by_curve_name(NID_sm2p256v1);
	EC_KEY_generate_key(sm2key);
	print_key(sm2key);

	EC_KEY *privateKey = EC_KEY_new();
	EC_KEY *publicKey = EC_KEY_new();
	const EC_GROUP *ec_group = EC_KEY_get0_group(sm2key);

	//2.私钥
	EC_KEY_set_group(privateKey, ec_group);
	EC_KEY_set_private_key(privateKey, EC_KEY_get0_private_key(sm2key));
	print_key(privateKey);

	//3.公钥
	EC_KEY_set_group(publicKey, ec_group);
	EC_KEY_set_public_key(publicKey, EC_KEY_get0_public_key(sm2key));
	print_key(publicKey);

	unsigned char msg[] = "hello world this is the message";
	printf("plaintext:");
	print_text(msg, strlen((char*)msg));

	//4.公钥加密
	unsigned char enMsg[512] = { 0 };
	size_t enLen = sizeof(enMsg);
	SM2_encrypt_with_recommended(msg, strlen((char*)msg), enMsg, &enLen, publicKey);
	printf("cipher text:");
	print_text(enMsg, enLen);

	//5.解密	
	unsigned char deMsg[512] = { 0 };
	size_t deLen = sizeof(enMsg);
	SM2_decrypt_with_recommended(enMsg, enLen, deMsg, &deLen, privateKey);
	printf("decrypt text:");
	print_text(deMsg, deLen);

	
	unsigned char sig[256];
	unsigned int siglen;
	siglen = sizeof(sig);
	int type = NID_sm_scheme;
	unsigned char dgst[EVP_MAX_MD_SIZE]="hello world";
	size_t dgstlen=strlen((char*)dgst);
	printf("dgst text:");
	print_text(dgst, dgstlen);

	//6.签名
	SM2_sign(type, dgst, dgstlen, sig, &siglen, privateKey);
	printf("sign:");
	print_text(sig, siglen);

	//7.核实
	if (!SM2_verify(type, dgst, dgstlen, sig, siglen, publicKey)) {
		return;
	}

	//释放资源
	EC_KEY_free(sm2key);
	EC_KEY_free(privateKey);
	EC_KEY_free(publicKey);
}