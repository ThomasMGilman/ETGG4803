#include "math_functions.h"

template<typename T, typename C>
T Math_Functions::norm(std::list<T>* vars, C p)
{
	T val = 0;
	for (T num : vars)
		val += pow(num, p);
	return pow(val, 1 / p);
}