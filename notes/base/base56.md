# base56

有些是有需要对一些数字进行压缩，可以采用base56或其他压缩。

```c++
#include "stdafx.h"
#include <string>
#include <iostream>
using namespace std;


const string CODES = "23456789abcdefghjklmnpqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";

// 01iIoO
string toBase32(unsigned int value) {
  string str;

  do {
    str.insert(0, string(1, CODES[value % 32]));
    value /= 32;
  } while (value > 0);

  return str;
}

unsigned int toBase10(string value) {
  std::reverse(value.begin(), value.end());

  unsigned int ret = 0;
  int count = 1;
  for (char& character : value) {
    ret += CODES.find(character) * count;
    count *= 32;
  }

  return ret;
}

int main()
{
  unsigned int val = 4294967295;
  std::cout << "val=" << val << std::endl;

  std::cout << "valstr=" << toBase32(val) << std::endl;


    return 0;
}

```