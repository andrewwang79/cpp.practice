#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

// nullptr begin
template <class T>
T clone(const T& t) {
  return t;
}
void g(int*) { std::cout << "函数 g 已调用" << std::endl; }

void _nullptr() {
  std::cout << "--------_nullptr--------" << std::endl;
  g(nullptr);  // 良好
  g(NULL);     // 良好
  g(0);        // 良好

  g(clone(nullptr));  // 良好
  // g(clone(NULL));    // 错误：非字面量的零不能为空指针常量
  // g(clone(0));       // 错误：非字面量的零不能为空指针常量
}
// nullptr end

// typedef begin
void _typedef() {
  std::cout << "--------_typedef--------" << std::endl;
  typedef int int_t, *intp_t, (&fp)(int, ulong), arr_t[10];

  // 下列两个对象拥有同一类型
  int a1[10];
  arr_t a2;
  a2[0] = 1;
}
// typedef end

// template begin
template <typename T, typename U>
class MagicType {
 public:
  T dark;
  U magic;
};
template <typename T>
using TrueDarkMagic = MagicType<std::vector<T>, std::string>;

template <typename T = int, typename U = int>
auto add(T x, U y) -> decltype(x + y) {
  return x + y;
}
template <typename T, typename U>
auto add3(T x, U y) {
  return x + y;
}
template <typename... Ts>
void magic(Ts... args) {
  std::cout << sizeof...(args) << std::endl;
}

template <typename... T>
auto sum(T... t) {
  return (t + ...);
}

void _template() {
  std::cout << "--------_template--------" << std::endl;
  auto ret = add<int, int>(1, 3);
  auto ret2 = add(5, 2);
  auto ret3 = add3(5, 2);
  TrueDarkMagic<bool> you;

  magic();       // 输出0
  magic(1);      // 输出1
  magic(1, "");  // 输出2

  std::cout << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << std::endl;
}
// template end

// misc begin
template <typename T>
auto print_type_info(const T& t) {
  if constexpr (std::is_floating_point<T>::value) {
    return t + 1;
  } else {
    return t + 0.001;
  }
}
int xyz(int a, int b, int c) { std::cout << a + b + c << std::endl; }
void _misc() {
  std::cout << "--------_misc--------" << std::endl;
  std::cout << print_type_info(5) << std::endl;
  std::cout << print_type_info(3.14) << std::endl;

  std::vector<int> vec = {1, 2, 3, 4};
  const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 2);
  if (itr != vec.end()) {
    *itr = 3;
  }
  for (auto element : vec) {
    std::cout << element << std::endl;  // read only
  }
  for (auto& element : vec) {
    element += 1;  // writeable
  }
  for (auto element : vec) {
    std::cout << element << std::endl;  // read only
  }

  // 将参数1,2绑定到函数 foo 上，但是使用 std::placeholders::_1
  // 来对第一个参数进行占位
  auto bindXyz = std::bind(xyz, std::placeholders::_1, 1, 2);
  // 这时调用 bindFoo 时，只需要提供第一个参数即可
  bindXyz(1);
}
// misc end

// rvalue begin
void reference(int& v) { std::cout << "左值引用" << std::endl; }
void reference(int&& v) { std::cout << "右值引用" << std::endl; }
template <typename T>
void pass(T&& v) {
  std::cout << "              普通传参: ";
  reference(v);
  std::cout << "       std::move 传参: ";
  reference(std::move(v));
  std::cout << "    std::forward 传参: ";
  reference(std::forward<T>(v));
  std::cout << "static_cast<T&&> 传参: ";
  reference(static_cast<T&&>(v));
}
void _rvalue() {
  std::cout << "--------_rvalue--------" << std::endl;
  std::cout << "传递右值:" << std::endl;
  pass(1);

  std::cout << "传递左值:" << std::endl;
  int v = 1;
  pass(v);
}
// rvalue end

// lambda begin
void _lambda() {
  std::cout << "--------_lambda--------" << std::endl;
  auto important = std::make_unique<int>(1);
  auto add = [v1 = 1, v2 = std::move(important)](int x, int y) -> int {
    return x + y + v1 + (*v2);
  };
  std::cout << add(3, 4) << std::endl;
}
// lambda end

// pointer begin
struct Foo {
  Foo() { std::cout << "Foo::Foo" << std::endl; }
  ~Foo() { std::cout << "Foo::~Foo" << std::endl; }
  void foo() { std::cout << "Foo::foo" << std::endl; }
};

void foo(std::shared_ptr<int> i) { (*i)++; }
void _pointer() {
  std::cout << "--------_pointer--------" << std::endl;
  auto pointer = std::make_shared<int>(10);
  foo(pointer);
  std::cout << *pointer << std::endl;  // 11

  auto pointeru = std::make_unique<Foo>();
  pointeru->foo();
  // auto pointer2 = pointeru;  // 非法
}

// pointer end

// thread begin
void _thread() {
  std::cout << "--------_thread--------" << std::endl;
  std::thread t([]() { std::cout << "hello world." << std::endl; });
  t.join();
}
// thread end

int main() {
  _misc();
  _nullptr();
  _typedef();
  _template();
  _rvalue();
  _lambda();
  _pointer();
  _thread();
  getchar();
}
