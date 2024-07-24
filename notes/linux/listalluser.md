# 获取所有登陆用户
## 1、shell方案
使用[listalluser](./listalluser.sh)

## 2、c++中调用shell脚本
## c98
定义一个宏，再调用。下面是示例。
```c++
#define SHELLSCRIPT "\
#/bin/bash \n\
echo -e \"\" \n\
echo -e \"This is a test shell script inside C code!!\" \n\
read -p \"press <enter> to continue\" \n\
clear\
"
```

## c11
定义原字符串，再调用。
```c++
inline const char* LISTALLUSER = R"(
#!/bin/bash
_l="/etc/login.defs"
_p="/etc/passwd"
 
## get mini UID limit ##
l=$(grep "^UID_MIN" $_l)
 
## get max UID limit ##
l1=$(grep "^UID_MAX" $_l)
 
## use awk to print if UID >= $MIN and UID <= $MAX and shell is not /sbin/nologin   ##
## echo "----------[ Normal User Accounts ]---------------"
awk -F':' -v "min=${l##UID_MIN}" -v "max=${l1##UID_MAX}" '{ if ( $3 >= min && $3 <= max  && $7 != "/sbin/nologin" ) print $1 }' "$_p"
)";
```

## 使用
```c++
std::vector<std::string> getNormalUsers() {
  std::vector<std::string> users;
  std::string ret = Helper::ssystem(LISTALLUSER);
  if(!ret.empty()) {
    users = Split(ret, "\n");
  }

  users.push_back("root");

  return users;
}
```