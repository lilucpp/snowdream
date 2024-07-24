#pragma once

extern "C" {

#include <openssl/evp.h>

	//SM3 接口示例

	//1.调用evp接口
	void evp_ec();

	//2.简单接口调用
	void evp_sm2();

	void createsm2();
	void test_readkeyfromfile();
	void testSignVrify();
	void testVrify();

	//3.命令行方式,example
	//	#SM2公私钥对
	// ./gmssl ecparam -genkey -name sm2p256v1 -out sm2key.pem
	//	#使用私钥签名
	//	./gmssl pkeyutl -sign -pkeyopt ec_sign_algor:sm2 -inkey signkey.pem -in demo -out demo.sig
	//	#从私钥中提取公钥
	//	./gmssl pkey -pubout -in sm2key.pem -out publicKey.pem
	//	#用公钥验证签名
	//	./gmssl pkeyutl -verify -pkeyopt ec_sign_algor:sm2 -in demo -sigfile demo.sig -pubin -inkey vrfykey.pem
	//	#加密
	// ./gmssl.exe pkeyutl -encrypt -in demo -pubin -pkeyopt ec_encrypt_algor:sm2 -inkey publicKey.pem -out demo.en
	// ./gmssl pkeyutl -decrypt -pkeyopt ec_encrypt_algor:sm2 -inkey sm2key.pem -in demo.en -out demo.de


	EVP_PKEY *genpkey(int curve_nid);
	int evp_pkey_encrypt_decrypt(EVP_PKEY *pkey);
	int evp_pkey_sign(EVP_PKEY *pkey);
	int evp_sign(EVP_PKEY *pkey, const EVP_MD *md);
}