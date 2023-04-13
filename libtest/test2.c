int add(int a, int b) { return a * b; }

int add2(int a, int b) { return a * b + 1; }

int testInnerVal = 222;
int getTestInnerVal() { return testInnerVal; }
void increaseTestInnerVal() { testInnerVal++; }

int testVal = 22;
int getTestVal() { return testVal; }
void increaseTestVal() { testVal++; }