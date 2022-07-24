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

// 场景：函数的返回值是类实例，如果调用方有用到，类实例在assign时不会析构和构造，待调用方用完后会析构。
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
  cout << "!!!!!!!!!!!!!!xyz.p address:" << &xyz.p << endl;
  return xyz;
}
void classReturnUsage() {
  Xyz_destructor xyz = _classReturnUsage();
  // 同函数内的地址和值
  cout << "!!!!!!!!!!!!!!xyz.p[1]:" << xyz.p[1] << endl;
  cout << "!!!!!!!!!!!!!!xyz.p address:" << &xyz.p << endl;
}

// 场景：new，泄露15字节(基于new的数组)
// 解决：delete[] a; delete[] q;
void charLeak() {
  char* a = new char[10];   // 值不初始化
  char* b = new char[5]();  // 值初始化成0
  a[0] = 'x';
  b[0] = 'y';

  cout << "!!!!!!!!!!!!!!a:" << a << endl;
  cout << "!!!!!!!!!!!!!!b:" << b << endl;
  // delete[] a;
  // delete[] b;
}

int main(int argc, char* argv[]) {
  classLeak();
  return 0;
}