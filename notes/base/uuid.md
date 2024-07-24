# uuid

## 简介
通用唯一识别码（英语：Universally Unique Identifier，缩写：UUID）是用于计算机体系中以识别信息的一个128位标识符。

## 格式
在其规范的文本表示中，UUID 的 16 个 8 位字节表示为 32 个十六进制数字，由连字符 '-' 分隔成五组显示，形式为“8-4-4-4-12”总共 36 个字符（32 个十六进制数字和 4 个连字符）。

例如：

123e4567-e89b-12d3-a456-426655440000


### 版本1（日期时间和MAC地址）
版本1的UUID，是根据 60-bit 的时间戳和节点（生成UUID的计算机）的48-bit MAC地址而生成的。

### 版本2（日期时间和MAC地址，DCE安全版本）
RFC 4122 保留了版本2的UUID用于“DCE security”；但并没有提供任何细节。因此，许多 UUID 实现省略了“版本2”。但是，“版本2”UUID 的规范由 DCE 1.1 身份验证和安全服务规范提供[4]。

### 版本3和版本5（基于命名空间名称）
“版本3”和“版本5”的 UUID 透过散列（hashing）命名空间标识符和名称生成。版本3使用 MD5 作为散列算法，版本5则使用 SHA1[6]。

### 版本4（随机）
版本4的UUID是随机生成的。


### 常见uuid命名空间
```c++
namespace ns {

BOOST_FORCEINLINE uuid dns() {
    uuid result = {{
        0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1 ,
        0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }};
    return result;
}

BOOST_FORCEINLINE uuid url() {
    uuid result = {{
        0x6b, 0xa7, 0xb8, 0x11, 0x9d, 0xad, 0x11, 0xd1 ,
        0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }};
    return result;
}

BOOST_FORCEINLINE uuid oid() {
    uuid result = {{
        0x6b, 0xa7, 0xb8, 0x12, 0x9d, 0xad, 0x11, 0xd1 ,
        0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }};
    return result;
}

BOOST_FORCEINLINE uuid x500dn() {
    uuid result = {{
        0x6b, 0xa7, 0xb8, 0x14, 0x9d, 0xad, 0x11, 0xd1 ,
        0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }};
    return result;
}
```

## boost示例代码

```c++
std::string customUUID5(const std::string& str) {
  std::string md5;
  peanut::md5String(str.c_str(), md5);
  boost::uuids::string_generator strgen;
  boost::uuids::uuid myns = strgen(md5);
  boost::uuids::name_generator gen(myns);
  boost::uuids::uuid id = gen(str);

  return boost::uuids::to_string(id);
}
```


## 在线uuid  
https://www.javainuse.com/uidv5

## c++在线编译器
可以选择boost库  
https://godbolt.org/  


## 参考
https://en.wikipedia.org/wiki/Universally_unique_identifier  
https://en.wikipedia.org/wiki/Universally_unique_identifier  