#include <iostream>
#include <testLibX/MathFunctions.h>

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	double aa = 2;
	double result = power(aa, 4);
	cout << result << endl;
	return 0;
}
