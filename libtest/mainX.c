#include <stdio.h>

#include "test.h"
extern int getTestValX();
extern void increaseTestValX();

int main(int argc, char* argv[]) {
  printf("getTestVal()1 : val(11) = %d\n", getTestVal());
  printf("getTestValX()1 : val(11) = %d\n", getTestValX());
  increaseTestVal();
  printf("getTestVal()2 : val(12) = %d\n", getTestVal());
  printf("getTestValX()2 : val(12) = %d\n", getTestValX());
  increaseTestValX();
  printf("getTestVal()3 : val(13) = %d\n", getTestVal());
  printf("getTestValX()3 : val(13) = %d\n", getTestValX());

  return 0;
}