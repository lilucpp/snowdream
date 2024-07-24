# json
目前个人最常用的是nlohmann::json。<br>
- headonly
- 类STL访问

## 常用的技巧

### 从STL容器转换
https://github.com/nlohmann/json/tree/v3.11.2#conversion-from-stl-containers
#### 序列容器创建 JSON 数组
- std::array, std::vector, std::deque, std::forward_list
- std::list，std::set, std::multiset, std::unordered_set, std::unordered_multiset
    但在这些情况下，数组元素的顺序取决于元素在相应 STL 容器中的排序方式。

```c++
std::vector<int> c_vector {1, 2, 3, 4};
json j_vec(c_vector);
// [1, 2, 3, 4]

std::list<bool> c_list {true, true, false, true};
json j_list(c_list);
// [true, true, false, true]

std::array<unsigned long, 4> c_array {{1, 2, 3, 4}};
json j_array(c_array);
// [1, 2, 3, 4]

std::set<std::string> c_set {"one", "two", "three", "four", "one"};
json j_set(c_set); // only one entry for "one" is used
// ["four", "one", "three", "two"]

std::unordered_set<std::string> c_uset {"one", "two", "three", "four", "one"};
json j_uset(c_uset); // only one entry for "one" is used
// maybe ["two", "three", "four", "one"]

std::multiset<std::string> c_mset {"one", "two", "one", "four"};
json j_mset(c_mset); // both entries for "one" are used
// maybe ["one", "two", "one", "four"]
```
#### 关联的键值容器创建 JSON 对象
- std::map, std::multimap, std::unordered_map, std::unordered_multimap
    请注意，在多映射的情况下，JSON 对象中仅使用一个键，值取决于 STL 容器的内部顺序。
```c++
std::map<std::string, int> c_map { {"one", 1}, {"two", 2}, {"three", 3} };
json j_map(c_map);
// {"one": 1, "three": 3, "two": 2 }

std::unordered_map<const char*, double> c_umap { {"one", 1.2}, {"two", 2.3}, {"three", 3.4} };
json j_umap(c_umap);
// {"one": 1.2, "two": 2.3, "three": 3.4}

std::multimap<std::string, bool> c_mmap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
json j_mmap(c_mmap); // only one entry for key "three" is used
// maybe {"one": true, "two": true, "three": true}

std::unordered_multimap<std::string, bool> c_ummap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
json j_ummap(c_ummap); // only one entry for key "three" is used
// maybe {"one": true, "two": true, "three": true}
```

#### 任意类型转换
// https://github.com/nlohmann/json/tree/v3.11.2#arbitrary-types-conversions

```c++
#include  <iostream>
#include "nlohmann/json.hpp"
#include <vector>
#include <list>
#include <map>
#include <string>

using namespace std;
using namespace nlohmann;

namespace ns {
struct Example {  
    std::string str;  
    std::map<std::string, std::string> mp;  
    std::vector<int> vec;  
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Example, str, mp, vec)
}

int main() {
    ns::Example ex{"lilu", {{"peanut", "lixiaoyu"},{"maodou", "unknown"}}, {1,2,3,4}};
    json j = ex;

    // {"mp":{"maodou":"unknown","peanut":"lixiaoyu"},"str":"lilu","vec":[1,2,3,4]}
    std::cout << "json=" << j.dump() << std::endl;

    return 0;
}
```