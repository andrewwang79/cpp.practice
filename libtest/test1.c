int add(int a, int b) { return a + b; }

int add1(int a, int b) { return a + b + 1; }
int add2(int a, int b) { return a + b + 2; }

int testInnerVal = 111;
int getTestInnerVal() { return testInnerVal; }
void increaseTestInnerVal() { testInnerVal++; }

int testVal = 11;
int getTestVal() { return testVal; }
void increaseTestVal() { testVal++; }

#pragma data_seg("OsSpace")
int testOsVal = 1027;
#pragma data_seg()
#pragma comment(linker, "/SECTION:OsSpace,RWS")
int getTestOsVal() { return testOsVal++; }