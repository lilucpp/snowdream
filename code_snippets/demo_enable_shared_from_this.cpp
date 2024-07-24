#include <iostream>
#include <memory>
#include <thread>

template <typename T>
class shared_from_this_singleton : public std::enable_shared_from_this<T> {
 public:
  using shared_self = std::shared_ptr<T>;

  template <typename... Arg>
  static shared_self create(Arg&&... arg) {
    struct sub : public T {
      sub(Arg&&... arg) : T(std::forward<Arg>(arg)...) {}
    };

    return std::make_shared<sub>(std::forward<Arg>(arg)...);
  }
};

// 第一种方式
struct Best : std::enable_shared_from_this<Best> {
  std::shared_ptr<Best> getptr() { return shared_from_this(); }

  static std::shared_ptr<Best> create() {
    // Not using std::make_shared<Best> because the c'tor is private.
    return std::shared_ptr<Best>(new Best());
  }

  void test() {
    auto self = getptr();
    std::thread t([self]() {
      // balabala
      self->api();
      // balabala
    });
    t.detach();
  }

 private:
  Best() = default;

  void api() {
    // balabala
  }
};

// 第二种方式
struct Great : shared_from_this_singleton<Great> {
  std::shared_ptr<Great> getptr() { return shared_from_this(); }

  void test() {
    auto self = getptr();
    std::thread t([self]() {
      // balabala
      self->api();
      // balabala
    });
    t.detach();
  }

 private:
  Great() = default;
  ~Great() = default;

  void api() {
    // balabala
  }

  friend class shared_from_this_singleton<Great>;
};

int main() {
  {
    std::shared_ptr<Best> b = Best::create();
    b->test();
  }

  {
    std::shared_ptr<Great> b = Great::create();
    b->test();
  }

  return 0;
}