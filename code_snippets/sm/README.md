# 国密算法

## sm3

[sm3规范](https://datatracker.ietf.org/doc/html/draft-sca-cfrg-sm3-02)  
[国家密码管理局关于发布《SM3密码杂凑算法》公告](https://www.oscca.gov.cn/sca/xxgk/2010-12/17/content_1002389.shtml)  
  
SM3适用于商用密码应用中的数字签名和验证，是在SHA-256基础上改进实现的一种算法，其安全性和SHA-256相当。SM3和MD5的迭代过程类似，也采用Merkle-Damgard结构。消息分组长度为512位，摘要值长度为256位。


## 示例
```c++
#include <string.h>

#include <iostream>
#include <sstream>
#include <string>

#include "sm3.h"

std::string arr2hex(unsigned char* hash, int len) {
  std::stringstream ss;
  for (int i = 0; i < len; ++i) ss << std::hex << (int)hash[i];
  return ss.str();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage:" << argv[0] << " string\n";
    return 0;
  }
  unsigned char hash[32] = {};
  sm3((unsigned char*)argv[1], strlen(argv[1]), hash);
  std::cout << "sm3(" << argv[1] << ")=" << arr2hex(hash, sizeof(hash))
            << std::endl;

  return 0;
}
```
结果验证：  
sm3(abc)=66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0  
sm3(hello world)=44f061e69fa6fdfc290c494654a5dcc53da7e5c52b84ef93a9d67d3fff88  
sm3(peanut)=931669bc1bbdb766a36239201e1feeecaa94ebdf637a9246c5c76494b416c02c  


