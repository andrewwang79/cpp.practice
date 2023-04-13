#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test.h"

int main(int argc, char* argv[]) {
  int sleepSeconds = 0;
  if (argc == 2) { sleepSeconds = atoi(argv[1]); }
  printf("sleepSeconds = %d\n", sleepSeconds);

  printf("add = %d\n", add(2, 5));
  printf("add1 = %d\n", add1(2, 5));
  printf("add2 = %d\n", add2(2, 5));
  printf("testVal = %d\n", testVal);
  testVal = 100;
  printf("testVal is changed. expect value is 100, actual value is %d\n", testVal);
  printf("round1 : testInnerVal = %d\n", getTestInnerVal());
  printf("round2 : testInnerVal = %d\n", getTestInnerVal());
  printf("testOsVal = %d\n", getTestOsVal());
  printf("testOsVal is changed. expect value is 1028, actual value is %d\n", getTestOsVal());

  sleep(sleepSeconds);
  return 0;
}