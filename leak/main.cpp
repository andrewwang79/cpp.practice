#include <iostream>
using namespace std;

// 场景：类实例自身没delete，泄露4字节
// 解决：加delete abc
class Abc {
  int xyz = 0;
};
void classLeak() {
  Abc* abc = new Abc();
  // delete abc;
}

// 场景：类实例的属性没delete，泄露24字节(基于new的数组是20字节)
// 解决：类析构函数加delete，参见Xyz_destructor
class Xyz_noDestructor {
  public:
    int* a;
    char* p;
};
void classPropertyLeak() {
  Xyz_noDestructor xyz;
  xyz.a = new int;
  xyz.p = new char[20];
}

// 场景：函数的返回值是类实例A，如果调用方把A赋值给变量B【B是指针不算】则A不会析构，B和A是同一个地址，待调用方B无效后再析构。不赋值则A直接析构
// 【原理上实例A在函数结束后就必须析构，此处应该是编译器优化结果】
class Xyz_destructor {
  public:
    int* a;
    char* p;
  ~Xyz_destructor() {
    cout << "!!!!!!!!!!!!!!Xyz_destructor destructor" << endl;
    if (a != NULL)
      delete a;
    if (p != NULL)
      delete[] p;
  }
};
Xyz_destructor _classReturnUsage() {
  Xyz_destructor xyz;
  xyz.a = new int;
  *xyz.a = 123;
  xyz.p = new char[20];
  xyz.p[1] = 'a';
  cout << "!!!!!!!!!!!!!!xyz address:" << &xyz << endl;
  cout << "!!!!!!!!!!!!!!xyz.a address:" << &xyz.a << endl;
  cout << "!!!!!!!!!!!!!!xyz.p address:" << &xyz.p << endl;
  return xyz;
}
// Xyz_destructor* xyz2;
Xyz_destructor classReturnUsage() {
  Xyz_destructor xyz = _classReturnUsage();
  // xyz2 = &xyz;
  // 返回实例的地址和值同函数(_classReturnUsage)内的实例
  cout << "!!!!!!!!!!!!!!xyz address:" << &xyz << endl;
  cout << "!!!!!!!!!!!!!!xyz.a address:" << &xyz.a << endl;
  cout << "!!!!!!!!!!!!!!xyz.p address:" << &xyz.p << endl;
  cout << "!!!!!!!!!!!!!!xyz.p[1]:" << xyz.p[1] << endl;
  cout << "!!!!!!!!!!!!!!FINISH classReturnUsage" << endl;
  return xyz;
}

// 场景：new，泄露19字节(基于new的数组)
// 解决：delete;
void basicLeak() {
  char* a = new char[10];   // 值不初始化
  char* b = new char[5]();  // 值初始化成0
  int* c = new int;
  int d;
  int32_t d32;
  int64_t d64;
  a = "andrew";
  b[0] = 'x';
  *c = 99;
  cout << "!!!!!!!!!!!!!!a address:" << a << endl;
  cout << "!!!!!!!!!!!!!!b address:" << b << endl;
  cout << "!!!!!!!!!!!!!!c address:" << c << endl;
  cout << "!!!!!!!!!!!!!!c sizeof:" << sizeof(c) << endl; // 64位系统指针是8字节
  cout << "!!!!!!!!!!!!!!*c sizeof:" << sizeof(*c) << endl;
  cout << "!!!!!!!!!!!!!!*c:" << *c << endl;
  cout << "!!!!!!!!!!!!!!d sizeof:" << sizeof(d) << endl; // 一般系统默认是4字节
  cout << "!!!!!!!!!!!!!!d32 sizeof:" << sizeof(d32) << endl;
  cout << "!!!!!!!!!!!!!!d64 sizeof:" << sizeof(d64) << endl;
  // delete[] a;
  // delete[] b;
  // delete c;
}

int main(int argc, char* argv[]) {
  basicLeak();
  return 0;
}