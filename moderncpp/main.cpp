#include <cxxabi.h>

#include <string.h>
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

// type_info begin
template <typename T>
auto print_type_info(const T& t) {
  if constexpr (std::is_floating_point<T>::value) {
    return t + 1;
  } else {
    return t + 0.001;
  }
}
void _type_info() {
  std::cout << "--------_type_info--------" << std::endl;
  std::cout << print_type_info(5) << std::endl;
  std::cout << print_type_info(3.14) << std::endl;
}
// type_info end

// bind begin
int xyz(int a, int b, int c) {
  std::cout << a + b + c << std::endl;
  return 0;
}

class TestClass {
 public:
  int xyz(int a, int b, int c) {
    std::cout << a + b + c << std::endl;
    return 0;
  }
};

namespace ABC {
class TypeClass {};
}  // namespace ABC

void _bind() {
  std::cout << "--------_bind--------" << std::endl;
  std::cout << "FN:" << std::endl;
  // 绑定函数。参数1使用占位符，参数2值是1，参数3值是2。使用时只需要输入参数1
  auto bindXyz = std::bind(xyz, std::placeholders::_1, 1, 2);
  bindXyz(1);
  // 绑定类函数
  auto bindClassXyz =
      std::bind(&TestClass::xyz, new TestClass(), std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3);
  bindClassXyz(1, 2, 3);

  typedef std::function<void(int, int, int)> FnXyz;
  FnXyz xyz1 = xyz;
  xyz1(2, 3, 4);

  // typeid
  std::cout << typeid(ABC::TypeClass).name() << std::endl;  // N3ABC9TypeClassE
  std::cout << abi::__cxa_demangle(typeid(ABC::TypeClass).name(), nullptr, nullptr, nullptr) << std::endl;  // ABC::TypeClass
}
// bind end

// for begin
void _for() {
  std::cout << "--------_for--------" << std::endl;
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
}
// for end

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

class Xyz {
  public:
    int* a;
    char* p;
    std::string name;
  Xyz(std::string _name) {
    name = _name;
  }
  ~Xyz() {
    std::cout << "!!!!!!!!!!!!!!Xyz destructor " << name << std::endl;
    if (a != NULL)
      delete a;
    if (p != NULL)
      delete[] p;
  }
};

// 引用+1
void _smart_pointer_fn_var(std::shared_ptr<Xyz> ptr) {
  std::cout << "!!!!!!!!!!!!!!testcase3 ptr31 var use count : " << ptr.use_count() << std::endl;
}
// 引用不变
void _smart_pointer_fn_ref(std::shared_ptr<Xyz>& ptr) {
  std::cout << "!!!!!!!!!!!!!!testcase3 ptr31 ref use count : " << ptr.use_count() << std::endl;
}

void foo(std::shared_ptr<int> i) { (*i)++; }
void assignSmartpoint(std::shared_ptr<char*>& p) {
  int len = 128;
  // 赋值已有智能指针
  std::shared_ptr<char*> char1 = std::make_shared<char*>(new char[len]);
  char* buff1 = *char1.get();
  buff1[0] = 'A';
  buff1[1] = 0;
  p = char1;
  std::cout << "!!!!!!!!!!!!!!char1 use count : " << char1.use_count() << std::endl; // 2
  std::cout << "!!!!!!!!!!!!!!p value : " << *p << std::endl;

  // 赋值已有指针
  char * buff2 = new char[len];
  buff2[0] = 'B';
  buff2[1] = 0;
  p = std::make_shared<char*>(buff2);
  std::cout << "!!!!!!!!!!!!!!char1 use count : " << char1.use_count() << std::endl; // 1
  std::cout << "!!!!!!!!!!!!!!p value : " << *p << std::endl;
}

void _smart_pointer() {
  std::cout << "--------_smart_pointer--------" << std::endl;
  std::cout << "--------_smart_pointer-shared--------" << std::endl;
  auto int_pointer = std::make_shared<int>(10);
  foo(int_pointer);
  std::cout << *int_pointer << std::endl;  // 11

  // 智能指针进函数赋值，引用参数
  std::shared_ptr<char*> pChar2;
  assignSmartpoint(pChar2);
  std::cout << "!!!!!!!!!!!!!!pChar2 use count : " << pChar2.use_count() << std::endl; // 1
  std::cout << "!!!!!!!!!!!!!!pChar2 value : " << *pChar2 << std::endl;

  // 计数 begin
  std::shared_ptr<Xyz> ptr11(new Xyz("11"));  // 引用计数为1
	std::shared_ptr<Xyz> ptr12 = ptr11;   //  ptr12共享ptr11的指向对象，引用计数为2
  std::cout << "!!!!!!!!!!!!!!testcase1 ptr11 A use count : " << ptr11.use_count() << std::endl; // 2
  std::cout << "!!!!!!!!!!!!!!testcase1 ptr12 A use count : " << ptr12.use_count() << std::endl; // 2
	ptr12.reset();  // ptr12置为空
  std::cout << "!!!!!!!!!!!!!!testcase1 ptr11 B use count : " << ptr11.use_count() << std::endl; // 1
  std::cout << "!!!!!!!!!!!!!!testcase1 ptr12 B use count : " << ptr12.use_count() << std::endl; // 0
	ptr11.reset();  // ptr11置为空
  std::cout << "!!!!!!!!!!!!!!testcase1 ptr11 C use count : " << ptr11.use_count() << std::endl; // 0
  std::cout << "!!!!!!!!!!!!!!testcase1 ptr12 C use count : " << ptr12.use_count() << std::endl; // 0

  std::shared_ptr<Xyz> ptr21(new Xyz("21"));
	Xyz *ptr22 = new Xyz("22");
  std::cout << "!!!!!!!!!!!!!!testcase2 ptr21 A use count : " << ptr21.use_count() << std::endl; // 1
	ptr21.reset(ptr22);  // ptr21指向ptr22：会自动delete(ptr21)老指针，新指针(xyz22)会在出作用域后自动delete[因为套了智能指针]
  std::cout << "!!!!!!!!!!!!!!testcase2 ptr21 A use count : " << ptr21.use_count() << std::endl; // 1
  
  std::shared_ptr<Xyz> ptr31(new Xyz("31"));
  _smart_pointer_fn_var(ptr31);
  std::cout << "!!!!!!!!!!!!!!testcase3 ptr31 after var use count : " << ptr31.use_count() << std::endl;
  _smart_pointer_fn_ref(ptr31);
  // 计数 end

  std::cout << "--------_smart_pointer-unique--------" << std::endl;
  auto class_pointer = std::make_unique<Foo>();
  class_pointer->foo();
  // auto class_pointer2 = class_pointer; // 编译报错，需要用下面的move
  auto class_pointer2 = std::move(class_pointer); // class_pointer2获得内存所有权，class_pointer此时等于nullptr

}

// pointer end

// thread begin
void _thread() {
  std::cout << "--------_thread--------" << std::endl;
  std::thread t([]() { std::cout << "hello world." << std::endl; });
  t.join();
}
// thread end

#define report std::cout << "exception TODO " << __FILE__ << __LINE__ << std::endl;

int main() {
  report;

  _bind();
  _type_info();
  _for();
  _nullptr();
  _typedef();
  _template();
  _rvalue();
  _lambda();
  _smart_pointer();
  _thread();

  std::cout << "press ENTER key to quit" << std::endl;
  getchar();
}