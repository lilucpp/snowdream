#pragma once

	//SM3 接口示例

	//1.直接调用函数EVP_Digest()
	void evp_sm3();

	//2.提供了EVP_sm3()  EVP_MD对象，应用程序可以通过标准的EVP API计算SM3杂凑值，具体接口调用方式可参考man EVP_DigestInit
	void evp_digest();

	//3.命令行方式,example
	//		echo -n abc | gmssl dgst -sm3