#include <stdio.h>

extern int g_share;
extern int g_func(int a);

void test_global() {
  int a = 2;
  a = g_func(a);
  printf("expect a=6. a=%d\n", a);
  printf("expect g_share=3. g_share=%d\n", g_share);
}

void test_static() {
  int m = 1;
  m++;
  printf("m=%d\n", m);
  static int s = 1;  // 局部静态变量
  s++;
  printf("s=%d\n", s);
}

int main() {
  test_static();  // s=2
  test_static();  // s=3
  // test_global();
  return 0;
}
