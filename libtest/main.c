#include <stdio.h>
#include "test.h"
 
int main()
{
    printf("add = %d\n", add(2, 5));
    printf("add1 = %d\n", add1(2, 5));
    printf("add2 = %d\n", add2(2, 5));
    printf("testVal = %d\n", testVal);
    testVal = 100;
    printf("testVal is changed. expect value is 100, actual value is %d\n", testVal);
    printf("round1 : testInnerVal = %d\n", getTestInnerVal());
    printf("round2 : testInnerVal = %d\n", getTestInnerVal());
    return 0;
}