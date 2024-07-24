# SM2验签
目前发现 sm2 有很多实现，比如 gmssl, openssl 1.1.1 ,openssl 3.0 和各种代码库实现等等。实践中发现这些实现会出现不能互相验签的情况。
网上的资料，给出了一些 openssl 指令，但是没有标明 openssl 的版本，由于 openssl1.1.1 版本和 openssl 3 版本支持的参数有区别，会导致到具体环境可能执行出错的情况1（[macos](https://so.csdn.net/so/search?q=macos&spm=1001.2101.3001.7020) 上的openssl version 显示为 libressl ,还不支持sm2openssl 1.1.1 目前停止维护了）。  

- 签名值可能每次不相同，因为设置了不同的k值。如果key值一样，签名值是一样的。
- sm3不是简单的对字串进行sm3，是一个复杂的预处理过程(资料1)，具体可以参考代码中的：sm3_digest_with_preprocess。

- 在数据、公私钥一致的情况下，不同工具之间不能互相验签有三种原因：签名算法不同、userid 不同、摘要算法不同。

## openssl命令

### openssl3

```shell
openssl ecparam -genkey -name sm2 -out sm_ec.key
openssl ec -in sm_ec.key  -pubout -out sm_ec.pubkey
openssl ec -in sm_ec.key -text -noout

echo -n abc > sig.data
openssl dgst -sm3 ./sig.data
openssl pkeyutl -sign -in ./sig.data  -inkey sm_ec.key  -out sm_ec.sig -rawin -digest sm3

openssl pkeyutl -verify -pubin -in ./sig.data  -inkey sm_ec.pubkey  -sigfile sm_ec.sig  -rawin -digest sm3
```

### openssl 1.1.1

openssl不支持sm2-sm3系列。

```shell
openssl ecparam -genkey -name SM2 -out sm_ec.key
openssl ec -in sm_ec.key  -pubout -out sm_ec.pubkey
echo -n abc > sig.data
openssl dgst -sm3 ./sig.data
openssl pkeyutl -sign -in ./sig.data  -inkey sm_ec.key  -out sm_ec.sig
openssl pkeyutl -verify -pubin -in ./sig.data  -inkey sm_ec.pubkey  -sigfile sm_ec.sig
```

### 其他

注意一定加-n， 不然会多计算一个换行。

```shell
echo -n $SM3_VALUE | openssl dgst -sign ./sm_ec.key -out sm_ec.sig
openssl dgst -verify  ./sm_ec.pubkey  -signature  ./sm_ec.sig  ./sig.data
```

## sm2-sm3

```shell
#Ubuntu 22.04.1 LTS
#version
#openssl version
#OpenSSL 3.0.2 15 Mar 2022 (Library: OpenSSL 3.0.2 15 Mar 2022)
#生成秘钥
openssl ecparam -genkey -name SM2 -out sm_ec.key
openssl ec -in sm_ec.key -pubout -out sm_ec.pubkey
echo -n abc > sig.data
#pkeyutl 默认 userid 是空的，可通过 distid 指定。
openssl pkeyutl -sign -in sig.data -inkey sm_ec.key -out sm_ec.sig -rawin -digest sm3
openssl pkeyutl -verify -pubin -in sig.data -inkey sm_ec.pubkey -sigfile sm_ec.sig -rawin -digest sm3

# ****************************************************************** #
#dgst 这个 -sm3 好像不用加，默认就是sm3,默认 userid 是空的，可通过 distid 指定。
openssl dgst -sm3 -sign sm_ec.key -out dgstsig -sigopt distid:1234567812345678 sig.data
openssl dgst -sm3 -verify sm_ec.pubkey -signature dgstsig -sigopt distid:1234567812345678 sig.data
```

## DER编码
DER的编码范例
下面显示DER编码格式（皆16进制）的数据结构:

30 -- 標籤說明 SEQUENCE
13 -- octets長度

02 -- 標籤說明 INTEGER
01 -- octets長度
05 -- value

16 -- 標籤說明 IA5String
0e -- octets長度
41 6e 79 62 6f 64 79 20 74 68 65 72 65 3f -- value 
("Anybody there?" in ASCII)  
例如：
```txt
`304402202d243865fcdc7e685b0a96045eebcb4b1fa724bcfd7b9642061cc116c2143d6d02205c042622e3b8a36bdefe465461ee9abc460360c18e8b57fa019f60a9da22ecf7`  

30 -- 標籤說明 SEQUENCE
44 -- 后面数据的长度：4*16+4=71

02 -- 標籤說明 INTEGER
20 -- 数据的长度 2*16=32（2d243865fcdc7e685b0a96045eebcb4b1fa724bcfd7b9642061cc116c2143d6d）

紧接着是
02  -- 標籤說明 INTEGER
20 -- 数据的长度 2*16=32（5c042622e3b8a36bdefe465461ee9abc460360c18e8b57fa019f60a9da22ecf7）
```

## AI写的简单解析示例
```c++
#include <iostream>
#include <vector>
#include <iomanip>
#include <openssl/bn.h>

// 解析 DER 编码的数据
bool parseDERData(const std::string& derDataHex, std::vector<std::vector<unsigned char>>& result)
{
    // 将十六进制编码数据转换为字节流
    std::vector<unsigned char> derData;
    for (size_t i = 0; i < derDataHex.size(); i += 2)
    {
        std::string byteString = derDataHex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::strtoul(byteString.c_str(), nullptr, 16));
        derData.push_back(byte);
    }

    size_t pos = 0;

    // 检查是否以 SEQUENCE 标记开头
    if (derData[pos] != 0x30)
    {
        std::cerr << "Error: Invalid DER encoding. Expected SEQUENCE." << std::endl;
        return false;
    }
    ++pos; // 跳过 SEQUENCE 标记字节

    // 获取 SEQUENCE 的长度
    unsigned char seqLength = derData[pos++];
    if (seqLength != derData.size() - pos)
    {
        std::cerr << "Error: Length mismatch in DER encoding." << std::endl;
        return false;
    }

    // 解析 DER 编码数据
    while (pos < derData.size())
    {
        // 解析 INTEGER
        if (derData[pos] == 0x02)
        {
            ++pos; // 跳过标记字节
            unsigned char intLength = derData[pos++]; // 获取长度
            std::vector<unsigned char> value(derData.begin() + pos, derData.begin() + pos + intLength); // 获取值
            pos += intLength; // 移动到下一个元素
            // 将解析结果存储到 result 中
            result.push_back(value);
        }
        else
        {
            std::cerr << "Error: Unexpected type in DER encoding. Expected INTEGER." << std::endl;
            return false;
        }
    }

    return true;
}

int main()
{
    // 原始 DER 编码数据
    std::string derDataHex = "30440220407c529e901745b3b903821fea1a76ea1bd2481fc0ce613f0b191e8004492a0b0220694082961beeb9f49b29bcffdad0dbc7a2902d039c66f796f93f97aa2d0a632b";

    // 解析 DER 编码数据
    std::vector<std::vector<unsigned char>> result;
    if (parseDERData(derDataHex, result))
    {
        // 输出解析结果
        for (const auto& vec : result)
        {
            std::cout << "INTEGER: ";
            for (unsigned char byte : vec)
            {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(byte);
            }
            std::cout << std::dec << std::endl; // 恢复输出流的十进制模式
        }
    }

    return 0;
}

```

## 验证

### 工具

#### sm2在线验证工具

可以试用https://i.goto327.top/CryptTools/SM2.aspx 在线工具进行验证。

注意公钥=04+公钥X+公钥Y，可以指定ID。

#### ASN.1解析工具

签名后的值是如果是ASN.1可以试用在线工具进行提取。https://the-x.cn/encodings/Asn1.aspx

#### 二进制16进制转化工具

```shell
# 1. 二进制签名转成16进制字符串。 xxd -ps ./xxx
root@:~/demo# xxd -ps ./sm_ec.sig 
304402202d243865fcdc7e685b0a96045eebcb4b1fa724bcfd7b9642061c
c116c2143d6d02205c042622e3b8a36bdefe465461ee9abc460360c18e8b
57fa019f60a9da22ecf7
# 2. 16进制字符串转化成二进制文本 xxd -r -p ./hexstr bin
## 使用标准输入输出 
echo -n AAF6387B741CD802070FDC516DE90745B16F87E8EB65F0A3CCF91E5ABAA5DF92 | xxd -r -p
## 使用文件
root@:~/demo# rm hexstr 
root@:~/demo# echo -n AAF6387B741CD802070FDC516DE90745B16F87E8EB65F0A3CCF91E5ABAA5DF92 > hexstr
root@:~/demo# cat hexstr 
AAF6387B741CD802070FDC516DE90745B16F87E8EB65F0A3CCF91E5ABAA5DF92root@:~/demo# 
root@:~/demo# xxd -r -p ./hexstr bin
root@:~/demo# cat bin
ª𷺴ւۑm腱o骥𤍹Zº¥ߒroot@:~/demo# 
root@:~/demo# xxd -ps ./bin 
aaf6387b741cd802070fdc516de90745b16f87e8eb65f0a3ccf91e5abaa5
df92
root@:~/demo# 
# 3. 二进制转化成c数组格式。
root@:~ /demo# xxd -i ./bin
unsigned char __bin[] = {
  0xaa, 0xf6, 0x38, 0x7b, 0x74, 0x1c, 0xd8, 0x02, 0x07, 0x0f, 0xdc, 0x51,
  0x6d, 0xe9, 0x07, 0x45, 0xb1, 0x6f, 0x87, 0xe8, 0xeb, 0x65, 0xf0, 0xa3,
  0xcc, 0xf9, 0x1e, 0x5a, 0xba, 0xa5, 0xdf, 0x92
};
unsigned int __bin_len = 32;
root@:~/demo# 
```

转化脚本**h2i.sh**

```shell
root@:~/temp# ls
h2i.sh
root@:~/temp# cat h2i.sh 
#!/bin/bash

# hex 2 bin
echo -n $1 | xxd -r -p | xxd -i 
root@:~/temp# 
```

#### 示例

```txt
公钥
0475c0b23b0a7b3c3cb84018dc42e3f1005d4cb5261bb00350152b72d392c8a3fff6bb6c8d6f3ac966d9e209774eabb3489ea3516f0ce2e9d75c39d6a7d1285333
  0x04, 0x75, 0xc0, 0xb2, 0x3b, 0x0a, 0x7b, 0x3c, 0x3c, 0xb8, 0x40, 0x18,
  0xdc, 0x42, 0xe3, 0xf1, 0x00, 0x5d, 0x4c, 0xb5, 0x26, 0x1b, 0xb0, 0x03,
  0x50, 0x15, 0x2b, 0x72, 0xd3, 0x92, 0xc8, 0xa3, 0xff, 0xf6, 0xbb, 0x6c,
  0x8d, 0x6f, 0x3a, 0xc9, 0x66, 0xd9, 0xe2, 0x09, 0x77, 0x4e, 0xab, 0xb3,
  0x48, 0x9e, 0xa3, 0x51, 0x6f, 0x0c, 0xe2, 0xe9, 0xd7, 0x5c, 0x39, 0xd6,
  0xa7, 0xd1, 0x28, 0x53, 0x33


签名 s r
3046022100
da21b4c00a22103c7d41f19570aadfba2813ade634c75820c45980a0dd6612f2
022100
91bb08871a6dfd36f7c069a37fcd4d1054484852ec6f538e219a6eebf46b82d2

----->
 <SEQUENCE>
  <INTEGER>0x00DA21B4C00A22103C7D41F19570AADFBA2813ADE634C75820C45980A0DD6612F2</INTEGER>
  <INTEGER>0x0091BB08871A6DFD36F7C069A37FCD4D1054484852EC6F538E219A6EEBF46B82D2</INTEGER>
 </SEQUENCE>

da21b4c00a22103c7d41f19570aadfba2813ade634c75820c45980a0dd6612f2
  0xda, 0x21, 0xb4, 0xc0, 0x0a, 0x22, 0x10, 0x3c, 0x7d, 0x41, 0xf1,
  0x95, 0x70, 0xaa, 0xdf, 0xba, 0x28, 0x13, 0xad, 0xe6, 0x34, 0xc7, 0x58,
  0x20, 0xc4, 0x59, 0x80, 0xa0, 0xdd, 0x66, 0x12, 0xf2

91bb08871a6dfd36f7c069a37fcd4d1054484852ec6f538e219a6eebf46b82d2
  0x91, 0xbb, 0x08, 0x87, 0x1a, 0x6d, 0xfd, 0x36, 0xf7, 0xc0, 0x69,
  0xa3, 0x7f, 0xcd, 0x4d, 0x10, 0x54, 0x48, 0x48, 0x52, 0xec, 0x6f, 0x53,
  0x8e, 0x21, 0x9a, 0x6e, 0xeb, 0xf4, 0x6b, 0x82, 0xd2

da21b4c00a22103c7d41f19570aadfba2813ade634c75820c45980a0dd6612f2

```

## c++代码

[SM2-signature-creation-and-verification](https://github.com/lilucpp/SM2-signature-creation-and-verification)

```c++
int test_with_input_defined_in_standard(void)
{
	int error_code;
	unsigned char msg[] = {"abc"};//{"message digest"};
	unsigned int msg_len = (unsigned int)(strlen((char *)msg));
	unsigned char user_id[] = {"1234567812345678"};
	unsigned int user_id_len = (unsigned int)(strlen((char *)user_id));
	unsigned char pub_key[65] =
	{
		0x04, 0x75, 0xc0, 0xb2, 0x3b, 0x0a, 0x7b, 0x3c, 0x3c, 0xb8, 0x40, 0x18,
	0xdc, 0x42, 0xe3, 0xf1, 0x00, 0x5d, 0x4c, 0xb5, 0x26, 0x1b, 0xb0, 0x03,
	0x50, 0x15, 0x2b, 0x72, 0xd3, 0x92, 0xc8, 0xa3, 0xff, 0xf6, 0xbb, 0x6c,
	0x8d, 0x6f, 0x3a, 0xc9, 0x66, 0xd9, 0xe2, 0x09, 0x77, 0x4e, 0xab, 0xb3,
	0x48, 0x9e, 0xa3, 0x51, 0x6f, 0x0c, 0xe2, 0xe9, 0xd7, 0x5c, 0x39, 0xd6,
	0xa7, 0xd1, 0x28, 0x53, 0x33
	};
		/*
		{0x04, 0x09, 0xf9, 0xdf, 0x31, 0x1e, 0x54, 0x21, 0xa1,
																 0x50, 0xdd, 0x7d, 0x16, 0x1e, 0x4b, 0xc5, 0xc6,
																 0x72, 0x17, 0x9f, 0xad, 0x18, 0x33, 0xfc, 0x07,
																 0x6b, 0xb0, 0x8f, 0xf3, 0x56, 0xf3, 0x50, 0x20,
																 0xcc, 0xea, 0x49, 0x0c, 0xe2, 0x67, 0x75, 0xa5,
																 0x2d, 0xc6, 0xea, 0x71, 0x8c, 0xc1, 0xaa, 0x60,
																 0x0a, 0xed, 0x05, 0xfb, 0xf3, 0x5e, 0x08, 0x4a,
																 0x66, 0x32, 0xf6, 0x07, 0x2d, 0xa9, 0xad, 0x13};*/
		unsigned char pri_key[32] =
	{
	 0xf8, 0xc2, 0x57, 0x62, 0x68, 0x3c, 0xb1, 0x7e, 0x6e, 0x99, 0xad, 0xc5,
	0x3a, 0xba, 0x59, 0xb4, 0x14, 0xc1, 0xc8, 0x6d, 0x91, 0x0b, 0xb1, 0x1f,
	0x42, 0x67, 0xe0, 0x13, 0x7a, 0x9d, 0xb6, 0x57
	};
	/*{0x39, 0x45, 0x20, 0x8f, 0x7b, 0x21, 0x44, 0xb1,
	                             0x3f, 0x36, 0xe3, 0x8a, 0xc6, 0xd3, 0x9f, 0x95,
	                             0x88, 0x93, 0x93, 0x69, 0x28, 0x60, 0xb5, 0x1a,
	                             0x42 ,0xfb, 0x81, 0xef, 0x4d, 0xf7, 0xc5, 0xb8};*/
	SM2_SIGNATURE_STRUCT sm2_sig;
	unsigned char std_r[32] = { 
	0xda, 0x21, 0xb4, 0xc0, 0x0a, 0x22, 0x10, 0x3c, 0x7d, 0x41, 0xf1,
	0x95, 0x70, 0xaa, 0xdf, 0xba, 0x28, 0x13, 0xad, 0xe6, 0x34, 0xc7, 0x58,
	0x20, 0xc4, 0x59, 0x80, 0xa0, 0xdd, 0x66, 0x12, 0xf2
	};
	/*{0xf5, 0xa0, 0x3b, 0x06, 0x48, 0xd2, 0xc4 ,0x63,
		                   0x0e, 0xea, 0xc5, 0x13, 0xe1, 0xbb, 0x81, 0xa1,
											 0x59, 0x44, 0xda, 0x38, 0x27, 0xd5, 0xb7, 0x41,
											 0x43, 0xac, 0x7e, 0xac, 0xee, 0xe7, 0x20, 0xb3};*/
	unsigned char std_s[32] = {
	0x91, 0xbb, 0x08, 0x87, 0x1a, 0x6d, 0xfd, 0x36, 0xf7, 0xc0, 0x69,
	0xa3, 0x7f, 0xcd, 0x4d, 0x10, 0x54, 0x48, 0x48, 0x52, 0xec, 0x6f, 0x53,
	0x8e, 0x21, 0x9a, 0x6e, 0xeb, 0xf4, 0x6b, 0x82, 0xd2

	};
	
	/*{0xb1, 0xb6, 0xaa, 0x29, 0xdf, 0x21, 0x2f, 0xd8,
														0x76, 0x31, 0x82, 0xbc, 0x0d, 0x42, 0x1c, 0xa1,
																				 0xbb, 0x90, 0x38, 0xfd, 0x1f, 0x7f, 0x42, 0xd4,
																				 0x84, 0x0b, 0x69, 0xc4, 0x85, 0xbb, 0xc1, 0xaa};*/
	memcpy(sm2_sig.r_coordinate, std_r, sizeof(std_r));
	memcpy(sm2_sig.s_coordinate, std_s, sizeof(std_s));

	printf("\n\n");


	if ( error_code = sm2_verify_sig(msg,
		                         msg_len,
					 user_id,
					 user_id_len,
					 pub_key,
					 &sm2_sig) )
	{
	   printf("Verify SM2 signature created by using input defined in standard failed!\n");
	   return error_code;
	}
	printf("Verify SM2 signature created by using input defined in standard succeeded!\n");

	return 0;
}
```



## 资料

- [SM2 签名预处理操作](https://blog.csdn.net/hpu11/article/details/106734216)
- https://blog.csdn.net/c2682736/article/details/132784663
- [DER](https://zh.wikipedia.org/wiki/ASN.1)