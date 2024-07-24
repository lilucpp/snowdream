# gTest

[资料](https://www.cnblogs.com/coderzh/archive/2009/03/31/1426758.html)

[samples](https://github.com/google/googletest/blob/master/googletest/docs/samples.md)

### 前言

gtest中的所有断言相关的宏。 gtest中，断言的宏可以理解为分为两类，一类是ASSERT系列，一类是EXPECT系列。一个直观的解释就是：

1. ASSERT_* 系列的断言，当检查点失败时，退出当前函数（注意：并非退出当前案例）。

2. EXPECT_* 系列的断言，当检查点失败时，继续往下执行。

### 布尔值检查

| **Fatal assertion**            | **Nonfatal assertion**         | **Verifies**         |
| ------------------------------ | ------------------------------ | -------------------- |
| `ASSERT_TRUE(`*condition*`)`;  | `EXPECT_TRUE(`*condition*`)`;  | *condition* is true  |
| `ASSERT_FALSE(`*condition*`)`; | `EXPECT_FALSE(`*condition*`)`; | *condition* is false |

### 数值型数据检查  

| **Fatal assertion**                    | **Nonfatal assertion**                 | **Verifies**             |
| -------------------------------------- | -------------------------------------- | ------------------------ |
| `ASSERT_EQ(`*expected*`, `*actual*`);` | `EXPECT_EQ(`*expected*`, `*actual*`);` | *expected* `==` *actual* |
| `ASSERT_NE(`*val1*`, `*val2*`);`       | `EXPECT_NE(`*val1*`, `*val2*`);`       | *val1* `!=` *val2*       |
| `ASSERT_LT(`*val1*`, `*val2*`);`       | `EXPECT_LT(`*val1*`, `*val2*`);`       | *val1* `<` *val2*        |
| `ASSERT_LE(`*val1*`, `*val2*`);`       | `EXPECT_LE(`*val1*`, `*val2*`);`       | *val1* `<=` *val2*       |
| `ASSERT_GT(`*val1*`, `*val2*`);`       | `EXPECT_GT(`*val1*`, `*val2*`);`       | *val1* `>` *val2*        |
| `ASSERT_GE(`*val1*`, `*val2*`);`       | `EXPECT_GE(`*val1*`, `*val2*`);`       | *val1* `>=` *val2*       |

#### 字符串检查  

| Fatal assertion  | Nonfatal assertion| Verifies  |
| --------- | ------ | ---------------------- |
| ASSERT_STREQ(expected_str, actual_str); | EXPECT_STREQ(expected_str, actual_str);| the two C strings have the same content |
| ASSERT_STRNE(str1, str2);| EXPECT_STRNE(str1, str2); | the two C strings have different content |
| ASSERT_STRCASEEQ(expected_str, actual_str); | EXPECT_STRCASEEQ(expected_str, actual_str); |the two C strings have the same content, ignoring case |
|ASSERT_STRCASENE(str1, str2);| EXPECT_STRCASENE(str1, str2); | the two C strings have different content, ignoring case |

*STREQ*和*STRNE*同时支持`char*和wchar_t*`类型的，*STRCASEEQ*和*STRCASENE*却只接收char*，估计是不常用吧。下面是几个例子：
```c++
TEST(StringCmpTest, Demo)
{
    char pszCoderZh  CoderZh;
    wchar_t wszCoderZh  LCoderZh;
    std:: strCoderZh  CoderZh;
    std::wstring wstrCoderZh  LCoderZh;

    EXPECT_STREQ(CoderZh, pszCoderZh);
    EXPECT_STREQ(LCoderZh, wszCoderZh);

    EXPECT_STRNE(CnBlogs, pszCoderZh);
    EXPECT_STRNE(LCnBlogs, wszCoderZh);

    EXPECT_STRCASEEQ(coderzh, pszCoderZh);
    //EXPECT_STRCASEEQ(L"coderzh", wszCoderZh);    不支持
    EXPECT_STREQ(CoderZh, strCoderZh.c_str());
    EXPECT_STREQ(LCoderZh, wstrCoderZh.c_str());
}
```

### 异常检查  

| **Fatal assertion**                              | **Nonfatal assertion**                           | **Verifies**                                      |
| ------------------------------------------------ | ------------------------------------------------ | ------------------------------------------------- |
| `ASSERT_THROW(`*statement*, *exception_type*`);` | `EXPECT_THROW(`*statement*, *exception_type*`);` | *statement* throws an exception of the given type |
| `ASSERT_ANY_THROW(`*statement*`);`               | `EXPECT_ANY_THROW(`*statement*`);`               | *statement* throws an exception of any type       |
| `ASSERT_NO_THROW(`*statement*`);`                | `EXPECT_NO_THROW(`*statement*`);`                | *statement* doesn't throw any exception           |

### 浮点型检查  

| **Fatal assertion**                       | **Nonfatal assertion**                    | **Verifies**                             |
| ----------------------------------------- | ----------------------------------------- | ---------------------------------------- |
| `ASSERT_FLOAT_EQ(`*expected, actual*`);`  | `EXPECT_FLOAT_EQ(`*expected, actual*`);`  | the two `float` values are almost equal  |
| `ASSERT_DOUBLE_EQ(`*expected, actual*`);` | `EXPECT_DOUBLE_EQ(`*expected, actual*`);` | the two `double` values are almost equal |

对相近的两个数比较：

| **Fatal assertion**                       | **Nonfatal assertion**                    | **Verifies**                                                 |
| ----------------------------------------- | ----------------------------------------- | ------------------------------------------------------------ |
| `ASSERT_NEAR(`*val1, val2, abs_error*`);` | `EXPECT_NEAR`*(val1, val2, abs_error*`);` | the difference between *val1* and *val2* doesn't exceed the given absolute error |


















