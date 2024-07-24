#include <string>
#include <random>

std::string GenRandomString(const int len) {
  static char chrs[] = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  std::mt19937 generator{std::random_device{}()};
  std::uniform_int_distribution<int> distribution{0, sizeof(chrs)-2};
  std::string rand_str(len, '\0');
  for (auto &dis : rand_str) dis = chrs[distribution(generator)];

  return rand_str;
}