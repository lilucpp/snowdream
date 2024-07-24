# Cpp Core Guidelines

Guidelines Support Library：https://github.com/microsoft/GSL



## 哲学

###  1. 直接在代码中表达想法

##### 原因编译器不阅读注释（或设计文档），许多程序员也不阅读（始终如一）。代码中表达的内容已经定义了语义，并且（原则上）可以由编译器和其他工具检查。

example1：

```C++
class Date {
public:
    Month month() const;  // do
    int month();          // don't
    // ...
};
```

example2：

```c++
void f(vector<string>& v)
{
    string val;
    cin >> val;
    // ...
    int index = -1;                    // bad, plus should use gsl::index
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] == val) {
            index = i;
            break;
        }
    }
    // ...
  
    // ...
    auto p = find(begin(v), end(v), val);  // better
    // ...
  
}
```

一个设计良好的库比直接使用语言特性更好地表达意图（要做什么，而不仅仅是如何做）。

**C++ 程序员应该了解标准库的基础知识，并在适当的时候使用它。**多看看标准库。

example3:

```c++
change_speed(double s);   // bad: what does s signify?
// ...
change_speed(2.3);
```

更好的方法是明确双倍的含义（新速度或旧速度的增量？）和使用的单位：

```c++
change_speed(Speed s);    // better: the meaning of s is specified
// ...
change_speed(2.3);        // error: no unit
change_speed(23_m / 10s);  // meters per second
```

具体的_m这种用法可以参考c++11中的 “用户定义字面量（user-defined-literals）”

### 2. 表达意图

除非说明了某些代码的意图（例如，在名称或注释中），否则无法判断代码是否做了它应该做的事情。

```
gsl::index i = 0;
while (i < v.size()) {
    // ... do something with v[i] ...
}
```

`v`这里没有表达“只是”循环遍历 元素的意图。索引的实现细节是公开的（因此它可能被误用），并且`i`超出了循环的范围，这可能是也可能不是。读者不能仅从这部分代码中得知。

更好的是：

```c++
for (const auto& x : v) { /* do something with the value of x */ }
```

现在，没有明确提及迭代机制，循环对`const`元素的引用进行操作，因此不会发生意外修改。如果需要修改，请说：

```c++
for (auto& x : v) { /* modify x */ }
for_each(v, [](int x) { /* do something with the value of x */ });
```

### 3. 理想情况下，程序应该是静态类型安全的

理想情况下，程序将是完全静态（编译时）类型安全的。不幸的是，这是不可能的。常见问题解决方案：

- unions – use `variant` (in C++17)
- casts – minimize their use; templates can help
- array decay – use `span` (from the GSL)
- range errors – use `span`
- narrowing conversions – minimize their use and use `narrow` or `narrow_cast` (from the GSL) where they are necessary

span: https://github.com/martinmoene/span-lite

### 4. 比运行时检查更喜欢编译时检查

example1: 

```c++
// Int is an alias used for integers
int bits = 0;         // don't: avoidable code
for (Int i = 1; i; i <<= 1)
    ++bits;
if (bits < 32)
    cerr << "Int too small\n";
```

```c++
// Int is an alias used for integers
static_assert(sizeof(Int) >= 4);    // do: compile-time check
```

或者最好还是使用类型系统并替换`Int`为`int32_t`

example2:

```c++
void read(int* p, int n);   // read max n integers into *p

int a[100];
read(a, 1000);    // bad, off the end
```

更好的

```c++
void read(span<int> r); // read into the range of integers r

int a[100];
read(a);        // better: let the compiler figure out the number of elements
```

**替代方案**：不要将在编译时可以做得很好的事情推迟到运行时。

### 5. 编译时不能检查的，运行时可以检查

example1, bad

```c++
// separately compiled, possibly dynamically loaded
extern void f(int* p);

void g(int n)
{
    // bad: the number of elements is not passed to f()
    f(new int[n]);
}
```

example2,bad

我们当然可以将元素的数量与指针一起传递：

```c++
// separately compiled, possibly dynamically loaded
extern void f2(int* p, int n);

void g2(int n)
{
    f2(new int[n], m);  // bad: a wrong number of elements can be passed to f()
}
```

example3，bad

标准库资源管理指针指向对象时无法传递大小：

```c++
// separately compiled, possibly dynamically loaded
// NB: this assumes the calling code is ABI-compatible, using a
// compatible C++ compiler and the same stdlib implementation
extern void f3(unique_ptr<int[]>, int n);

void g3(int n)
{
    f3(make_unique<int[]>(n), m);    // bad: pass ownership and size separately
}
```

example4

我们需要将指针和元素数量作为一个整数对象传递：

```c++
extern void f4(vector<int>&);   // separately compiled, possibly dynamically loaded
extern void f4(span<int>);      // separately compiled, possibly dynamically loaded
                                // NB: this assumes the calling code is ABI-compatible, using a
                                // compatible C++ compiler and the same stdlib implementation

void g3(int n)
{
    vector<int> v(n);
    f4(v);                     // pass a reference, retain ownership
    f4(span<int>{v});          // pass a view, retain ownership
}
```

example5

我们如何转让所有权和验证使用所需的所有信息？

```c++
vector<int> f5(int n)    // OK: move
{
    vector<int> v(n);
    // ... initialize v ...
    return v;
}

unique_ptr<int[]> f6(int n)    // bad: loses n
{
    auto p = make_unique<int[]>(n);
    // ... initialize *p ...
    return p;
}

owner<int*> f7(int n)    // bad: loses n and we might forget to delete
{
    owner<int*> p = new int[n];
    // ... initialize *p ...
    return p;
}
```

### 6. 不要泄露任何资源

随着时间的推移，即使资源的缓慢增长也会耗尽这些资源的可用性。这对于长时间运行的程序尤其重要，但它是负责任的编程行为的重要组成部分。

example

```c++
void f(char* name)
{
    FILE* input = fopen(name, "r");
    // ...
    if (something) return;   // bad: if something == true, a file handle is leaked
    // ...
    fclose(input);
}

// 首选RAII :
void f(char* name)
{
    ifstream input {name};
    // ...
    if (something) return;   // OK: no leak
    // ...
}
```

## 接口

### 1. 使接口显式

example1:

通过全局（命名空间范围）变量（调用模式）控制函数的行为是隐式的，并且可能会造成混淆。

```c++
int round(double d)
{
    return (round_up) ? ceil(d) : d;    // don't: "invisible" dependency
}
```

- （简单）函数不应根据在命名空间范围内声明的变量的值做出控制流决策。
- （简单）函数不应写入在命名空间范围内声明的变量。

### 2. 避免非`const`全局变量

非`const`全局变量隐藏了依赖关系并使依赖关系发生不可预测的变化。

```c++
struct Data {
    // ... lots of stuff ...
} data;            // non-const data

void compute()     // don't
{
    // ... use data ...
}

void output()     // don't
{
    // ... use data ...
}
```

规则是“避免”，而不是“不要使用”。当然会有（罕见的）例外，比如`cin`, `cout`, and `cerr`.

### 3. 避免单例

单例基本上是伪装的复杂全局对象。

您可以使用最简单的“单例”（简单到通常不被认为是单例）在第一次使用时进行初始化，如果有的话：

```c++
X& myX()
{
    static X my_x {3};
    return my_x;
}
```

这是与初始化顺序相关的问题的最有效解决方案之一。在多线程环境中，静态对象的初始化不会引入竞争条件（除非您不小心从其构造函数中访问共享对象）。

请注意，本地的初始化`static`并不意味着竞争条件。但是，如果销毁`X`涉及需要同步的操作，我们必须使用不那么简单的解决方案。例如：

```c++
X& myX()
{
    static auto p = new X {3};
    return *p;  // potential leak
}
```

现在有人必须`delete`以某种适当的线程安全方式来处理该对象。这很容易出错，所以我们不使用这种技术。

### 4. 使接口精确且强类型

类型是最简单和最好的文档，由于其定义明确的含义而提高了易读性，并在编译时进行检查。此外，精确类型的代码通常会得到更好的优化。

不要考虑：

```c++
void pass(void* data);    // weak and under qualified type void* is suspicious
```

不要考虑：

```c++
draw_rect(100, 200, 100, 500); // what do the numbers specify?

draw_rect(p.x, p.y, 10, 20); // what units are 10 and 20 in?
```

注释和参数名称会有所帮助，但我们可以明确表示：

```c++
void draw_rectangle(Point top_left, Point bottom_right);
void draw_rectangle(Point top_left, Size height_width);

draw_rectangle(p, Point{10, 20});  // two corners
draw_rectangle(p, Size{10, 20});   // one corner and a (height, width) pair
```

显然，我们无法通过静态类型系统捕获所有错误（例如，第一个参数应该是左上角的事实留给约定（命名和注释））。

不要考虑：

```c++
set_settings(true, false, 42); // what do the numbers specify?
```

参数类型及其值不会传达指定的设置或这些值的含义。

这种设计更加明确、安全、易读：

```c++
alarm_settings s{};
s.enabled = true;
s.displayMode = alarm_settings::mode::spinning_light;
s.frequency = alarm_settings::every_10_seconds;
set_settings(s);
```

对于一组布尔值，请考虑使用标志枚举；表示一组布尔值的模式。

```c++
enable_lamp_options(lamp_option::on | lamp_option::animate_state_transitions);
```

在下面的例子中，从界面`time_to_blink`上看不清楚是什么意思：秒？毫秒？

```c++
void blink_led(int time_to_blink) // bad -- the unit is ambiguous
{
    // ...
    // do something with time_to_blink
    // ...
}

void use()
{
    blink_led(2);
}
```

`std::chrono::duration` 类型有助于明确持续时间的单位。

```c++
void blink_led(milliseconds time_to_blink) // good -- the unit is explicit
{
    // ...
    // do something with time_to_blink
    // ...
}

void use()
{
    blink_led(1500ms);
}
```

该函数也可以编写为接受任何持续时间单位。

```c++
template<class rep, class period>
void blink_led(duration<rep, period> time_to_blink) // good -- accepts any unit
{
    // assuming that millisecond is the smallest relevant unit
    auto milliseconds_to_blink = duration_cast<milliseconds>(time_to_blink);
    // ...
    // do something with milliseconds_to_blink
    // ...
}

void use()
{
    blink_led(2s);
    blink_led(1500ms);
}
```

### 5. 陈述先决条件（如果有的话）

参数的含义可能会限制它们在被调用方中的正确使用。

一些先决条件可以表示为断言。例如：

```c++
double sqrt(double x) { Expects(x >= 0); /* ... */ }
```

### 6. 优先`Expects()`表达前提条件

明确条件是前提条件并启用工具使用。

```c++
int area(int height, int width)
{
    Expects(height > 0 && width > 0);            // good
    if (height <= 0 || width <= 0) my_error();   // 不清晰
    // ...
}
```

`if`先决条件可以通过多种方式来表述，包括注释、-语句和`assert()`. 这会使它们难以与普通代码区分开来，难以更新，难以被工具操纵，并且可能具有错误的语义（您是否总是想在调试模式下中止并在生产运行中不检查任何内容？）。

`Expects()` 也可用于检查算法中间的条件。

### 7. 状态后置条件

检测对结果的误解并可能发现错误的实现。

```c++
int area(int height, int width) { return height * width; }  // bad
```
考虑使用：
```c++
int area(int height, int width)
{
    auto res = height * width;
    Ensures(res > 0);
    return res;
}
```

考虑一个著名的安全漏洞：

```c++
void f()    // problematic
{
    char buffer[MAX];
    // ...
    memset(buffer, 0, sizeof(buffer));
}
```

没有后置条件表明应该清除缓冲区并且优化器消除了明显冗余的`memset()`调用：

```c++
void f()    // better
{
    char buffer[MAX];
    // ...
    memset(buffer, 0, sizeof(buffer));
    Ensures(buffer[0] == 0);
}
```

### 8. 永远不要通过原始指针 ( `T*`) 或引用 ( `T&`)转移所有权

### 9. 声明一个不能为空的指针为 `not_null`

帮助避免取消引用`nullptr`错误。通过避免对`nullptr`.

```c++
int length(const char* p);            // it is not clear whether length(nullptr) is valid

length(nullptr);                      // OK?

int length(not_null<const char*> p);  // better: we can assume that p cannot be nullptr

int length(const char* p);            // we must assume that p can be nullptr
```

通过在源代码中说明意图，实现者和工具可以提供更好的诊断，例如通过静态分析发现某些类别的错误，并执行优化，例如删除分支和空测试。

### 10. 避免全局对象的复杂初始化

复杂的初始化可能导致未定义的执行顺序。

```c++
// file1.c

extern const X x;

const Y y = f(x);   // read x; write y

// file2.c

extern const Y y;

const X x = g(y);   // read y; write x
```

由于`x`和`y`在不同的翻译单元中，调用`f()`和的顺序`g()`是不确定的；一个人将访问一个未初始化的`const`. 这表明全局（命名空间范围）对象的初始化顺序问题不仅限于全局*变量*。

初始化顺序问题在并发代码中变得特别难以处理。通常最好完全避免全局（命名空间范围）对象。

### 11. 首选空抽象类作为类层次结构的接口

空的抽象类（没有非静态成员数据）比有状态的基类更稳定。

```c++
class Shape {    // better: Shape is a pure interface
public:
    virtual Point center() const = 0;   // pure virtual functions
    virtual void draw() const = 0;
    virtual void rotate(int) = 0;
    // ...
    // ... no data members ...
    // ...
    virtual ~Shape() = default;
};
```

### 12. 如果你想要一个交叉编译器 ABI，使用 C 风格的子集

不同的编译器为类、异常处理、函数名称和其他实现细节实现不同的二进制布局。

### 13. 对于稳定的库 ABI，请考虑 Pimpl idiom

因为私有数据成员参与类布局，私有成员函数参与重载解析，对这些实现细节的更改需要使用它们的类的所有用户重新编译。持有指向实现的指针 (Pimpl) 的非多态接口类可以以间接为代价将类的用户与其实现的更改隔离开来。

界面（小部件.h）

```c++
class widget {
    class impl;
    std::unique_ptr<impl> pimpl;
public:
    void draw(); // public API that will be forwarded to the implementation
    widget(int); // defined in the implementation file
    ~widget();   // defined in the implementation file, where impl is a complete type
    widget(widget&&); // defined in the implementation file
    widget(const widget&) = delete;
    widget& operator=(widget&&); // defined in the implementation file
    widget& operator=(const widget&) = delete;
};
```

实现（小部件.cpp）

```c++
class widget::impl {
    int n; // private data
public:
    void draw(const widget& w) { /* ... */ }
    impl(int n) : n(n) {}
};
void widget::draw() { pimpl->draw(*this); }
widget::widget(int n) : pimpl{std::make_unique<impl>(n)} {}
widget::widget(widget&&) = default;
widget::~widget() = default;
widget& widget::operator=(widget&&) = default;
```

## Function

### 1. 一个函数应该执行一个单一的逻辑操作

### 2. 保持函数简短

### 3. 如果一个函数可能需要在编译时求值，声明它 `constexpr`

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

- 不要试图使所有功能`constexpr`。大多数计算最好在运行时完成。

- 任何最终可能依赖于高级运行时配置或业务逻辑的 API 都不应该被制作`constexpr`。编译器无法评估此类自定义，并且任何`constexpr`依赖于该 API 的函数都必须重构或删除`constexpr`。

- 不可能和不必要的。如果`constexpr`在需要常量的地方调用非函数，编译器会给出错误。

### 4. 如果函数非常小且时间紧迫，则声明它 `inline`

一些优化器擅长在没有程序员提示的情况下内联，但不要依赖它。措施！在过去 40 年左右的时间里，我们一直承诺编译器可以在没有人类提示的情况下比人类更好地内联。我们还在等。指定内联（在类定义中编写成员函数时显式或隐式）鼓励编译器做得更好。

```
inline string cat(const string& s, const string& s2) { return s + s2; }
```

不要把一个`inline`函数放在一个稳定的接口中，除非你确定它不会改变。内联函数是 ABI 的一部分。

`constexpr`暗示`inline`.

### 5. 如果你的函数不能抛出，声明它 `noexcept`

如果不应该抛出异常，则不能假定程序可以处理错误，应尽快终止。声明一个函数`noexcept`通过减少替代执行路径的数量来帮助优化器。它还可以加快失败后的退出速度。

##### Example

 Put `noexcept` on every function written completely in C or in any other language without exceptions. The C++ Standard Library does that implicitly for all functions in the C Standard Library.

`noexcept`甚至可以在可以抛出的函数上使用：

```c++
vector<string> collect(istream& is) noexcept
{
    vector<string> res;
    for (string s; is >> s;)
        res.push_back(s);
    return res;
}
```

### 6. 对于一般用途，采用`T*`或`T&`参数而不是智能指针

传递智能指针会转移或共享所有权，并且仅应在需要所有权语义时使用。不操作生命周期的函数应该使用原始指针或引用.

##### Example, good

```c++
// callee
void f(widget& w)
{
    // ...
    use(w);
    // ...
};

// caller
shared_ptr<widget> my_widget = /* ... */;
f(*my_widget);

widget stack_widget;
f(stack_widget); // ok -- now this works
```

### 7. 未使用的参数应该是未命名的

可读性。抑制未使用的参数警告。

```c++
X* find(map<Blob>& m, const string& s, Hint);   // once upon a time, a hint was used
```

### 8. 对于“in”参数，通过值传递廉价复制的类型，通过引用传递其他类型 `const`

```c++
void f1(const string& s);  // OK: pass by reference to const; always cheap

void f2(string s);         // bad: potentially expensive

void f3(int x);            // OK: Unbeatable

void f4(const int& x);     // bad: overhead on access in f4()
```

避免“深奥的技术”，例如：

- 将参数传递为`T&&`“为了效率”。大多数关于性能优势的传言`&&`都是虚假的或脆弱的（但见[F.18](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-consume)和[F.19](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-forward)）。

### 9. 对于“in-out”参数，通过引用传递给非`const`

这让调用者清楚地知道该对象被认为是被修改的。

```c++
void update(Record& r);  // assume that update writes to r
```

### 10. 对于“will-move-from”参数，pass by`X&&`和`std::move`参数

它是有效的，并消除了调用站点的错误：`X&&`绑定到右值，`std::move`如果传递左值，则需要在调用站点显式。

```c++
void sink(vector<int>&& v)  // sink takes ownership of whatever the argument owned
{
    // usually there might be const accesses of v here
    store_somewhere(std::move(v));
    // usually no more use of v here; it is moved-from
}
```

请注意，这`std::move(v)`使得以移动状态`store_somewhere()`离开成为可能`v`。 

### 11. 对于“转发”参数，`TP&&`只传递`std::forward`参数

```c++
template<class F, class... Args>
inline auto invoke(F f, Args&&... args)
{
    return f(forward<Args>(args)...);
}
```

### 12. 对于“out”输出值，比输出参数更喜欢返回值

返回值是自我记录的，而 a`&`可以是 in-out 或 out-only，并且容易被误用。

这包括大型对象，例如使用隐式移动操作来提高性能并避免显式内存管理的标准容器。

如果要返回多个值，请使用元组或类似的多成员类型。

```c++
// OK: return pointers to elements with the value x
vector<const int*> find_all(const vector<int>&, int x);

// Bad: place pointers to elements with value x in-out
void find_all(const vector<int>&, vector<const int*>& out, int x);
```

不建议返回`const`值。这种旧的建议现在已经过时了；它不会增加价值，并且会干扰移动语义。

```c++
const vector<int> fct();    // bad: that "const" is more trouble than it is worth

vector<int> g(const vector<int>& vx)
{
    // ...
    fct() = vx;   // prevented by the "const"
    // ...
    return fct(); // expensive copy: move semantics suppressed by the "const"
}
```

```c++
struct Package {      // exceptional case: expensive-to-move object
    char header[16];
    char load[2024 - 16];
};

Package fill();       // Bad: large return value
void fill(Package&);  // OK

int val();            // OK
void val(int&);       // Bad: Is val reading its argument

```

### 13 要返回多个“out”值，最好返回一个结构体或元组

返回值是自记录为“仅输出”值。请注意，根据使用 a `tuple`（包括`pair`）的约定，C++ 确实有多个返回值，可能`tie`在调用站点具有额外的便利或结构化绑定 (C++17)。首选使用命名结构，其中返回值具有语义。否则，无名`tuple`在通用代码中很有用。

```c++
// BAD: output-only parameter documented in a comment
int f(const string& input, /*output only*/ string& output_data)
{
    // ...
    output_data = something();
    return status;
}

// GOOD: self-documenting
tuple<int, string> f(const string& input)
{
    // ...
    return make_tuple(status, something());
}
```

C++98 的标准库已经使用了这种风格，因为 a`pair`就像一个二元素`tuple`。例如，给定 a `set<string> my_set`，请考虑：

```c++
// C++98
result = my_set.insert("Hello");
if (result.second) do_something_with(result.first);    // workaround
```

使用 C++11 我们可以这样写，将结果直接放在现有的局部变量中：

```c++
Sometype iter;                                // default initialize if we haven't already
Someothertype success;                        // used these variables for some other purpose

tie(iter, success) = my_set.insert("Hello");   // normal return value
if (success) do_something_with(iter);
```

在 C++17 中，我们可以使用“结构化绑定”来声明和初始化多个变量：

```c++
if (auto [ iter, success ] = my_set.insert("Hello"); success) do_something_with(iter);
```

例外有时，我们需要将一个对象传递给一个函数来操纵它的状态。在这种情况下，通过引用传递对象[`T&`](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-inout)通常是正确的技术。通常不需要显式地将输入输出参数作为返回值再次传回。例如：

```c++
istream& operator>>(istream& is, string& s);    // much like std::operator>>()

for (string s; cin >> s; ) {
    // do something with line
}
```

在许多情况下，返回特定的、用户定义的类型会很有用。例如：

```c++
struct Distance {
    int value;
    int unit = 1;   // 1 means meters
};

Distance d1 = measure(obj1);        // access d1.value and d1.unit
auto d2 = measure(obj2);            // access d2.value and d2.unit
auto [value, unit] = measure(obj3); // access value and unit; somewhat redundant
                                    // to people who know measure()
auto [x, y] = measure(obj4);        // don't; it's likely to be confusing
```

### 14. 使用`T*`或`owner<T*>`指定单个对象

```c++
void use(int* p, int n, char* s, int* q)
{
    p[n - 1] = 666; // Bad: we don't know if p points to n elements;
                    // assume it does not or use span<int>
    cout << s;      // Bad: we don't know if that s points to a zero-terminated array of char;
                    // assume it does not or use zstring
    delete q;       // Bad: we don't know if *q is allocated on the free store;
                    // assume it does not or use owner
}
```

更好的

```
void use2(span<int> p, zstring s, owner<int*> q)
{
    p[p.size() - 1] = 666; // OK, a range error can be caught
    cout << s; // OK
    delete q;  // OK
}
```

##### 笔记

##### `owner<T*>`代表所有权，`zstring`代表一个 C 风格的字符串。

**另外**：假设`T*`从指向`T`（例如`unique_ptr<T>`）的智能指针获得的 a 指向单个元素。

### 15. 使用 `not_null<T>`表示“null”不是有效值

考虑：

```
int length(Record* p);
```

当我打电话时，`length(p)`应该检查是否`p`是`nullptr`第一个？是否应该执行`length()`检查是否`p`是`nullptr`？

```c++
// it is the caller's job to make sure p != nullptr
int length(not_null<Record*> p);

// the implementor of length() must assume that p == nullptr is possible
int length(Record* p);
```

##### 笔记 

`not_null`不仅适用于内置指针。它适用于`unique_ptr`、`shared_ptr`和其他类似指针的类型。

### 16. 使用 `zstring`或 `not_null<zstring>`来指定 C 风格的字符串

### 17. Return `T&` from assignment operators

```c++ 
class Foo
{
 public:
    ...
    Foo& operator=(const Foo& rhs)
    {
      // Copy members.
      ...
      return *this;
    }
};

```

### 18. 不要 `return std::move(local)`

##### bad

```c++
S f()
{
  S result;
  return std::move(result);
}
```

##### good

```c++
S f()
{
  S result;
  return result;
}
```

### 19. 首选在将在本地使用的 lambda 中通过引用捕获，包括传递给算法

为了效率和正确性，在本地使用 lambda 时，您几乎总是希望通过引用进行捕获。这包括编写或调用本地并行算法时，因为它们在返回之前加入。

不幸的是，没有简单的方法可以通过引用`const`来获取本地调用的效率，同时还可以防止副作用。

在这里，一个大对象（网络消息）被传递给迭代算法，复制消息（可能不可复制）既不高效也不正确：

```c++
std::for_each(begin(sockets), end(sockets), [&message](auto& socket)
{
    socket.send(message);
});
```

### 20. **避免不必要的条件嵌套**

条件的浅嵌套使代码更易于遵循。这也使意图更加清晰。努力将基本代码放在最外层的范围内，除非这会掩盖意图。

使用保护条款来处理特殊情况并尽早返回。

```c++
// Bad: Deep nesting
void foo() {
    ...
    if (x) {
        computeImportantThings(x);
    }
}

// Bad: Still a redundant else.
void foo() {
    ...
    if (!x) {
        return;
    }
    else {
        computeImportantThings(x);
    }
}

// Good: Early return, no redundant else
void foo() {
    ...
    if (!x)
        return;

    computeImportantThings(x);
}
```

```c++
// Bad: Unnecessary nesting of conditions
void foo() {
    ...
    if (x) {
        if (y) {
            computeImportantThings(x);
        }
    }
}

// Good: Merge conditions + return early
void foo() {
    ...
    if (!(x && y))
        return;

    computeImportantThings(x);
}
```

## 类和类层次结构

### 1.仅当函数需要直接访问类的表示时才使其成为成员

### 2. 将辅助函数放在它们支持的类的同一个命名空间中

### 3. A base class destructor should be either public and virtual, or protected and non-virtual

### 4. 析构函数不能失败

一般来说，如果析构函数失败，我们不知道如何编写无错误的代码。标准库要求它处理的所有类都具有不通过抛出退出的析构函数。

```c++
class X {
public:
    ~X() noexcept;
    // ...
};

X::~X() noexcept
{
    // ...
    if (cannot_release_a_resource) terminate();
    // ...
}
```

- 声明一个析构函数`noexcept`。这将确保它正常完成或终止程序。

- 如果资源不能被释放并且程序不能失败，尝试以某种方式向系统的其余部分发出失败的信号（甚至可以通过修改一些全局状态并希望有什么东西会注意到并能够解决问题）。请充分意识到这种技术是特殊用途且容易出错的。

### 5. 不要定义只初始化数据成员的默认构造函数；改用类内成员初始值设定项

使用类内成员初始值设定项让编译器为您生成函数。编译器生成的函数可以更高效。

```c++
class X1 { // BAD: doesn't use member initializers
    string s;
    int i;
public:
    X1() :s{"default"}, i{1} { }
    // ...
};
```

```c++
class X2 {
    string s = "default";
    int i = 1;
public:
    // use compiler-generated default constructor
    // ...
};
```

### 6. 默认情况下，显式声明单参数构造函数

合理使用explicit。

如果您真的想要从构造函数参数类型到类类型的隐式转换，请不要使用`explicit`

### 7. 按成员声明的顺序定义和初始化成员变量

尽量减少混淆和错误。这就是初始化发生的顺序（与成员初始值设定项的顺序无关）。

```c++
class Foo {
    int m1;
    int m2;
public:
    Foo(int x) :m2{x}, m1{++x} { }   // BAD: misleading initializer order
    // ...
};

Foo x(1); // surprise: x.m1 == x.m2 == 2
```

### 8. 对于常量初始值设定项，在构造函数中优先使用类内初始值设定项而不是成员初始值设定项

明确要求在所有构造函数中使用相同的值。避免重复。避免维护问题。它导致最短和最有效的代码。

```c++
class X {   // BAD
    int i;
    string s;
    int j;
public:
    X() :i{666}, s{"qqq"} { }   // j is uninitialized
    X(int ii) :i{ii} {}         // s is "" and j is uninitialized
    // ...
};
```

维护者如何知道是否`j`故意未初始化（无论如何可能是个坏主意）以及是否有意在一种情况下和另一种情况下提供`s`默认值（几乎可以肯定是一个错误）？当一个新成员被添加到一个现有的类时，（忘记初始化一个成员）的问题经常发生。

```c++
class X2 {
    int i {666};
    string s {"qqq"};
    int j {0};
public:
    X2() = default;        // all members are initialized to their defaults
    X2(int ii) :i{ii} {}   // s and j initialized to their defaults
    // ...
};
```

**替代方案**：我们可以从构造函数的默认参数中获得部分好处，这在旧代码中并不少见。但是，这不太明确，会导致传递更多参数，并且当有多个构造函数时会重复：

```c++
class X3 {   // BAD: inexplicit, argument passing overhead
    int i;
    string s;
    int j;
public:
    X3(int ii = 666, const string& ss = "qqq", int jj = 0)
        :i{ii}, s{ss}, j{jj} { }   // all members are initialized to their defaults
    // ...
};
```

### 9. 在构造函数中优先于初始化而不是赋值

初始化明确指出已完成初始化而不是赋值，并且可以更加优雅和高效。防止“设置前使用”错误。

```c++
class A {   // Good
    string s1;
public:
    A(czstring p) : s1{p} { }    // GOOD: directly construct (and the C-string is explicitly named)
    // ...
};
```

```c++
class B {   // BAD
    string s1;
public:
    B(const char* p) { s1 = p; }   // BAD: default constructor followed by assignment
    // ...
};

class C {   // UGLY, aka very bad
    int* p;
public:
    C() { cout << *p; p = new int{10}; }   // accidental use before initialized
    // ...
};
```

`const char*`我们可以使用 C++17`std::string_view`或`gsl::span<char>` 作为[向函数提供参数的更通用方式来](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rstr-view)代替那些s ：

```c++
class D {   // Good
    string s1;
public:
    D(string_view v) : s1{v} { }    // GOOD: directly construct
    // ...
};
```

### 10. 使用委托构造函数来表示类的所有构造函数的通用操作

```c++
lass Date {   // BAD: repetitive
    int d;
    Month m;
    int y;
public:
    Date(int dd, Month mm, year yy)
        :d{dd}, m{mm}, y{yy}
        { if (!valid(d, m, y)) throw Bad_date{}; }

    Date(int dd, Month mm)
        :d{dd}, m{mm} y{current_year()}
        { if (!valid(d, m, y)) throw Bad_date{}; }
    // ...
};
```

常见的动作写起来很乏味，而且可能不小心不常见。

```c++
class Date2 {
    int d;
    Month m;
    int y;
public:
    Date2(int dd, Month mm, year yy)
        :d{dd}, m{mm}, y{yy}
        { if (!valid(d, m, y)) throw Bad_date{}; }

    Date2(int dd, Month mm)
        :Date2{dd, mm, current_year()} {}
    // ...
};
```

### 11. 使复制赋值为非`virtual`，取参数为`const&`，并由非返回`const&`

它简单而高效。如果您想针对右值进行优化，请提供一个采用 a 的重载`&&`（参见[F.18](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-consume)）。

```c++
class Foo {
public:
    Foo& operator=(const Foo& x)
    {
        // GOOD: no need to check for self-assignment (other than performance)
        auto tmp = x;
        swap(tmp); // see C.83
        return *this;
    }
    // ...
};

Foo a;
Foo b;
Foo f();

a = b;    // assign lvalue: copy
a = f();  // assign rvalue: potentially move
```

也可以参考String(陈硕)实现

```c++
#include <utility>
#include <string.h>

class String
{
 public:
  String()
    : data_(new char[1])
  {
    *data_ = '\0';
  }

  String(const char* str)
    : data_(new char[strlen(str) + 1])
  {
    strcpy(data_, str);
  }

  String(const String& rhs)
    : data_(new char[rhs.size() + 1])
  {
    strcpy(data_, rhs.c_str());
  }
  /* Delegate constructor in C++11
  String(const String& rhs)
    : String(rhs.data_)
  {
  }
  */

  ~String()
  {
    delete[] data_;
  }

  /* Traditional:
  String& operator=(const String& rhs)
  {
    String tmp(rhs);
    swap(tmp);
    return *this;
  }
  */
  String& operator=(String rhs) // yes, pass-by-value
  {
    swap(rhs);
    return *this;
  }

  // C++ 11
  String(String&& rhs)
    : data_(rhs.data_)
  {
    rhs.data_ = nullptr;
  }

  String& operator=(String&& rhs)
  {
    swap(rhs);
    return *this;
  }

  // Accessors

  size_t size() const
  {
    return strlen(data_);
  }

  const char* c_str() const
  {
    return data_;
  }

  void swap(String& rhs)
  {
    std::swap(data_, rhs.data_);
  }

 private:
  char* data_;
};
```

