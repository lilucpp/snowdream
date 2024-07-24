#pragma once

	//SMS4 接口示例

	//1.简单的加密方式
	//		调用sms4_encrypt、sms4_decrypt
	int evp_sms4();

	//2.SMS4的EVP_sms4_cbc()等EVP_CIPHER对象，应用可以通过EVP_EncryptInit/Update/Final()函数访问，具体调用方法请参考EVP_EncryptInit(3)手册页
	int evp_sms4_cbc();

	//3.命令行方式,example
	//		echo hello | gmssl enc -sms4-cbc > ciphertext.bin
	//		cat cipehrtext.bin | gmssl enc -sms4-cbc -d