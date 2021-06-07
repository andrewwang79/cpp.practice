#include "MathFunctions.h"

double power_wrong(double base, int exponent){
	return 6;
}
double power(double base, int exponent)
{
	int result = base;
	int i;

	if (exponent == 0) {
		return 1;
	}

	for (i = 1; i < exponent; ++i) {
		result = result * base;
	}
	return result;
}