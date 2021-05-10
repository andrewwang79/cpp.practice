#include <stdio.h>

int g_share;
extern int g_func(int a);

int main() {
    int a=2;
    a = g_func(a);
    printf("expect a=6. a=%d\n", a);
    printf("expect g_share=3. g_share=%d\n", g_share);
    return 1;
}