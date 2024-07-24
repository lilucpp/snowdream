#include "example_sm2.h"
#include <openssl/sm2.h>
#include <openssl/ec.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include "utility.h"


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