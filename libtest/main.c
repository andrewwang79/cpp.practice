#include <stdio.h>
#include "test.h"
 
int main()
{
    printf("add = %d\n", add(2, 5));
    printf("add1 = %d\n", add1(2, 5));
    printf("add2 = %d\n", add2(2, 5));
    printf("testVal = %d\n", testVal);
    testVal = 100;
    printf("testVal = %d\n", testVal);
    return 0;
}