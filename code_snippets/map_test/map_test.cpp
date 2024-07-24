#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <thread>

std::map<std::string, std::string> g_vals;
std::mutex g_mutext4vals;

std::string GenRandomString(const int len) {
  std::mt19937 generator{std::random_device{}()};
  // modify range according to your need "A-Z","a-z" or "0-9" or whatever you
  // need.
  std::uniform_int_distribution<int> distribution{'a', 'z'};
  std::string rand_str(len, '\0');
  for (auto& dis : rand_str) dis = static_cast<char>(distribution(generator));

  return rand_str;
}

void product(int num) {
  while (true) {
    for (int i = 0; i < num; ++i) {
      std::lock_guard<std::mutex> lock(g_mutext4vals);
      g_vals.emplace(GenRandomString(32), GenRandomString(64));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void consumer() {
  while (true) {
    auto start = std::chrono::steady_clock::now();
    std::map<std::string, std::string> vals;
    {
      std::lock_guard<std::mutex> lock(g_mutext4vals);
      vals = g_vals;
    }
    auto end = std::chrono::steady_clock::now();

    size_t total_len=0;
    for (auto& item : vals) {
      total_len += item.first.size();
      total_len += item.second.size();
    }

    auto du =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "consumer size=" << vals.size() << ", total_len=" << total_len
              << ", copy map =" << du.count() << "ms" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void resetvals() {
  while (true) {
    {
      std::lock_guard<std::mutex> lock(g_mutext4vals);
      // std::cout << "reset g_vals.size=" << g_vals.size() << std::endl;
      g_vals.clear();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "usage a.out num\n";
    return 1;
  }
  int num = atoi(argv[1]);
  std::vector<std::shared_ptr<std::thread>> ths;
  ths.emplace_back(new std::thread(std::bind(product, num)));
  ths.emplace_back(new std::thread(consumer));
  ths.emplace_back(new std::thread(resetvals));

  for (auto t : ths) t->join();

  return 0;
}