# [C++11](./CPP11_ref.md)

## Overview

C++11 includes the following new language features:

- [move semantics](#move-semantics)
- [variadic templates](#variadic-templates)
- [rvalue references](#rvalue-references)
- [forwarding references](#forwarding-references)
- [initializer lists](#initializer-lists)
- [static assertions](#static-assertions)
- [auto](#auto)
- [lambda expressions](#lambda-expressions)
- [decltype](#decltype)
- [template aliases](#template-aliases)
- [nullptr](#nullptr)
- [strongly-typed enums](#strongly-typed-enums)
- [attributes](#attributes)
- [constexpr](#constexpr)
- [delegating constructors](#delegating-constructors)
- [user-defined literals](#user-defined-literals)
- [explicit virtual overrides](#explicit-virtual-overrides)
- [final specifier](#final-specifier)
- [default functions](#default-functions)
- [deleted functions](#deleted-functions)
- [range-based for loops](#range-based-for-loops)
- [special member functions for move semantics](#special-member-functions-for-move-semantics)
- [converting constructors](#converting-constructors)
- [explicit conversion functions](#explicit-conversion-functions)
- [inline-namespaces](#inline-namespaces)
- [non-static data member initializers](#non-static-data-member-initializers)
- [right angle brackets](#right-angle-brackets)
- [ref-qualified member functions](#ref-qualified-member-functions)
- [trailing return types](#trailing-return-types)

C++11 includes the following new library features:
- [std::move](#stdmove)
- [std::forward](#stdforward)
- [std::thread](#stdthread)
- [std::to_string](#stdto_string)
- [type traits](#type-traits)
- [smart pointers](#smart-pointers)
- [std::chrono](#stdchrono)
- [tuples](#tuples)
- [std::tie](#stdtie)
- [std::array](#stdarray)
- [unordered containers](#unordered-containers)
- [std::make_shared](#stdmake_shared)
- [memory model](#memory-model)
- [std::async](#stdasync)

## C++11 Language Features

### move semantics

1. Motivation

   Move semantics is mostly about performance optimization: the ability to move an [expensive]( "昂贵的") object from one address in memory to another, while [pilfering]( "窃取") resources of the source in order to construct the target with minimum expense.

2. Copy vs Move

   The difference between a copy and a move is that a copy leaves the source unchanged. A move on the other hand leaves the source in a state defined differently for each type. From a client code point of view, choosing move instead of copy means that you **don't care what happens to the state of the source**. 

## Rvalue references

C++0x has introduced a new kind of reference type called rvalue references. Rvalue references help solve the problem of **unnecessary copying** and **enable perfect forwarding**. This reference type enables move semantics. When the right-hand side of an assignment is an rvalue, then the left-hand side object can **steal** resources from the right-hand side object rather than performing a separate allocation. Perfect forwarding allows you to write a single function template that takes N arbitrary arguments and forwards them transparently to another arbitrary function.

The only difference is in the referent: A regular non-const reference always points to a non-temporary, while an **rvalue reference always points to a temporary**.

## Forward references

 A forwarding reference is created with the syntax `T&&` where `T` is a template type parameter, or using `auto&&`. This enables two major features: move semantics; and *perfect forwarding*, the ability to pass arguments that are either lvalues or rvalues.

`auto` type deduction with lvalues and rvalues:

```c++
int x = 0; // `x` is an lvalue of type `int`
auto&& al = x; // `al` is an lvalue of type `int&` -- binds to the lvalue, `x`
auto&& ar = 0; // `ar` is an lvalue of type `int&&` -- binds to the rvalue temporary, `0`
```

Template type parameter deduction with lvalues and rvalues:

```c++
#include <iostream>
#include <typeinfo>
#include <utility>
using namespace std;

void foo(int& x) {
  cout << "call foo(int&) x=" << x << endl;
}

void foo(int&& x) {
  cout << "call foo(int&&) x=" << x << endl;
}

// Since C++11 or later:
template <typename T>
void f(T&& t) {
  //cout << "t=" << typeid(forward<T>(t)).name() << endl;
  foo(forward<T>(t));
  cout << endl;
}

int main() {
  int x = 0;
  f(0); // deduces as f(int&&)
  f(x); // deduces as f(int&)

  int& y = x;
  f(y); // deduces as f(int& &&) => f(int&)

  int&& z = 0; // NOTE: `z` is an lvalue with type `int&&`.
  z=1;
  f(z); // deduces as f(int&& &) => f(int&)
  f(std::move(z)); // deduces as f(int&& &&) => f(int&&)

  return 0;
}
```

### Initializer lists

A lightweight array-like container of elements created using a "braced list" syntax. For example, `{ 1, 2, 3 }` creates a sequences of integers, that has type `std::initializer_list<int>`. Useful as a replacement to passing a vector of objects to a function.

```c++
int sum(const std::initializer_list<int>& list) {
  int total = 0;
  for (auto& e : list) {
    total += e;
  }

  return total;
}

auto list = {1, 2, 3};
sum(list); // == 6
sum({1, 2, 3}); // == 6
sum({}); // == 0
```

### Static assertions

Performs compile-time assertion checking.

1. Syntax

   `static_assert ( bool_constexpr, message)`

2. Declaration Scopes

   static_assert can be used in namespace scope, class scope, as well as block scope.

   - Namespace scope

     ```c++
     #include <iostream> 
     static_assert(sizeof(void* ) == 8,  
     "DTAMDL(*LLP64) is not allowed for this module."); 
     int main() {
     	//...
     }
     ```
	
	- Class scope
	
	  ```c++
	  #include <iostream> 
	  using namespace std; 
	    
	  template <class T, int Size> 
	  class Vector { 
	      // Compile time assertion to check if 
	      // the size of the vector is greater than 
	      // 3 or not. If any vector is declared whose 
	      // size is less than 4, the assertion will fail 
	      static_assert(Size > 3, "Vector size is too small!"); 
	    
	      T m_values[Size]; 
	  }; 
	    
	  int main() { 
	      Vector<int, 4> four; // This will work 
	      Vector<short, 2> two; // This will fail 
	    
	      return 0; 
	  }
	  ```
	
	- Block scope
	
	  ```c++
	  template <typename T, int N> 
	  void f() { 
	      static_assert(N >= 0, "length of array a is negative."); 
	      T a[N]; 
	  } 
	  int main() { 
	      // assertion fails here 
	      // because the length of the array passed 
	      // is below 0 
	      f<int, -1>(); 
	      return 0; 
	  } 
	  ```
	
	  

### auto

`auto`-typed variables are deduced by the compiler according to the type of their initializer.

```
auto a = 3.14; // double
auto b = 1; // int
auto& c = b; // int&
auto d = { 0 }; // std::initializer_list<int>
auto&& e = 1; // int&&
auto&& f = b; // int&
auto g = new auto(123); // int*
const auto h = 1; // const int
auto i = 1, j = 2, k = 3; // int, int, int
auto l = 1, m = true, n = 1.61; // error -- `l` deduced to be int, `m` is bool
auto o; // error -- `o` requires initializer
```

Extremely useful for readability, especially for complicated types:

```
std::vector<int> v = ...;
std::vector<int>::const_iterator cit = v.cbegin();
// vs.
auto cit = v.cbegin();
```

Functions can also deduce the return type using `auto`. In C++11, a return type must be specified either explicitly, or using `decltype` like so:

```
template <typename X, typename Y>
auto add(X x, Y y) -> decltype(x + y) {
  return x + y;
}
add(1, 2); // == 3
add(1, 2.0); // == 3.0
add(1.5, 1.5); // == 3.0
```

The trailing return type in the above example is the *declared type* (see section on [`decltype`](https://github.com/lilucpp/modern-cpp-features/blob/master/CPP11.md#decltype)) of the expression `x + y`. For example, if `x` is an integer and `y` is a double, `decltype(x + y)` is a double. Therefore, the above function will deduce the type depending on what type the expression `x + y` yields. Notice that the trailing return type has access to its parameters, and `this` when appropriate.

### user-defined-literals

资料：

1. https://zh.cppreference.com/w/cpp/language/user_literal
2. https://zh.wikipedia.org/wiki/%E7%94%A8%E6%88%B7%E5%AE%9A%E4%B9%89%E5%AD%97%E9%9D%A2%E9%87%8F

- 通过定义用户定义的后缀，允许整数、浮点数、字符及字符串字面量产生用户定义类型的对象。

- 如果一个记号同时匹配用户定义字面量的语法和常规字面量的语法，则它被假定为常规字面量（即不可能重载 123LL 中的 LL）。

- 作为用户定义字面量所用的 *用户定义后缀* 的标识符，其将调用此函数。必须以下划线 `**_**` 开始：不以下划线开始的后缀为标准库提供的字面量运算符所保留。

```c++
#include <cstddef>
#include <algorithm>
#include <iostream>
#include <numbers>
#include <string>
 
// 用作从度（输入参数）转换为弧度（返回输出）
constexpr long double operator"" _deg_to_rad ( long double deg )
{
    long double radians = deg * std::numbers::pi_v<long double> / 180;
    return radians;
}
 
// 用作自定义类型
struct mytype
{
    unsigned long long m;
};
constexpr mytype operator"" _mytype ( unsigned long long n )
{
    return mytype{n};
}
 
// 用作副作用
void operator"" _print ( const char* str )
{
    std::cout << str << '\n';
}
 
#if __cpp_nontype_template_args < 201911
 
std::string operator"" _x2 ( const char* str, std::size_t )
{
    return std::string{str} + str;
}
 
#else // C++20 字符串字面量运算符模板
 
template<std::size_t N>
struct DoubleString
{
    char p[N*2-1]{};
 
    constexpr DoubleString ( char const(&pp)[N] )
    {
        std::ranges::copy(pp, p);
        std::ranges::copy(pp, p + N - 1);
    };
};
 
template<DoubleString A>
constexpr auto operator"" _x2()
{
    return A.p;
}
 
#endif // C++20
 
int main()
{
    double x_rad = 90.0_deg_to_rad;
    std::cout << std::fixed << x_rad << '\n';
 
    mytype y = 123_mytype;
    std::cout << y.m << '\n';
 
    0x123ABC_print;
    std::cout << "abc"_x2 << '\n';
}
```

输出：

```
1.570796
123
0x123ABC
abcabc
```



```c++
#include<iostream>
#include<string>
using namespace std;
struct S{
    S (const char * lls): value(lls){};
    string value;
};

template < char ... cdots> S operator "" _mysuffix()
{
    const char cv[] {cdots...,'\0'}; //把可变的模板参数用于初始化器（initializer）
    S  sv_(cv);
    return sv_;
}

int main()
{
   S sv {1234.567_mysuffix} ;
   std::cout<<sv.value<<std::endl;
   return 0;
}
```

### constexpr

`constexpr` 说明符声明编译时可以对函数或变量求值值。这些变量和函数（给定了合适的函数实参的情况下）即可用于需要编译期[常量表达式](https://zh.cppreference.com/w/cpp/language/constant_expression)的地方。

声明对象或非静态成员函数 (C++14 前)时使用 constexpr 说明符则同时蕴含 const。声明函数或[静态](https://zh.cppreference.com/w/cpp/language/static)成员变量 (C++17 起)时使用 constexpr 说明符则同时蕴含 inline。如果一个函数或函数模板的某个声明拥有 `constexpr` 说明符，则其所有声明都必须含有该说明符。

```c++
constexpr int square(int x) {
  return x * x;
}

int square2(int x) {
  return x * x;
}

int a = square(2);  // mov DWORD PTR [rbp-4], 4

int b = square2(2); // mov edi, 2
                    // call square2(int)
                    // mov DWORD PTR [rbp-8], eax
```

`constexpr` 需要告诉编译器允许编译时评估。

`constexpr`不保证编译时评估；如果程序员需要它或编译器决定这样做来优化，它只是保证可以在编译时为常量表达式参数评估函数。

```c++
constexpr int min(int x, int y) { return x < y ? x : y; }

void test(int v)
{
    int m1 = min(-1, 2);            // probably compile-time evaluation
    constexpr int m2 = min(-1, 2);  // compile-time evaluation
    int m3 = min(-1, v);            // run-time evaluation
    constexpr int m4 = min(-1, v);  // error: cannot evaluate at compile time
}
```



```c++
#include <iostream>
#include <stdexcept>
 
// C++11 constexpr 函数使用递归而非迭代
// （C++14 constexpr 函数可使用局部变量和循环）
constexpr int factorial(int n)
{
    return n <= 1 ? 1 : (n * factorial(n - 1));
}
 
// 字面类
class conststr {
    const char* p;
    std::size_t sz;
public:
    template<std::size_t N>
    constexpr conststr(const char(&a)[N]): p(a), sz(N - 1) {}
 
    // constexpr 函数通过抛异常来提示错误
    // C++11 中，它们必须用条件运算符 ?: 来这么做
    constexpr char operator[](std::size_t n) const
    {
        return n < sz ? p[n] : throw std::out_of_range("");
    }
    constexpr std::size_t size() const { return sz; }
};
 
// C++11 constexpr 函数必须把一切放在单条 return 语句中
// （C++14 无此要求）
constexpr std::size_t countlower(conststr s, std::size_t n = 0,
                                             std::size_t c = 0)
{
    return n == s.size() ? c :
           'a' <= s[n] && s[n] <= 'z' ? countlower(s, n + 1, c + 1) :
                                        countlower(s, n + 1, c);
}
 
// 输出要求编译时常量的函数，用于测试
template<int n>
struct constN
{
    constN() { std::cout << n << '\n'; }
};
 
int main()
{
    std::cout << "4! = " ;
    constN<factorial(4)> out1; // 在编译时计算
 
    volatile int k = 8; // 使用 volatile 防止优化
    std::cout << k << "! = " << factorial(k) << '\n'; // 运行时计算
 
    std::cout << "\"Hello, world!\" 里小写字母的个数是 ";
    constN<countlower("Hello, world!")> out2; // 隐式转换到常量字符串
}
```

输出：

```
4! = 24
8! = 40320
"Hello, world!" 里小写字母的个数是 9
```













