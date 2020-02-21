#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <list>

using namespace std;

int get_random_int(int max, int min = 0)
{
	return rand() % max + min;
}

///Returns the string version of the value passed in
template<class T>
typename enable_if<is_fundamental<T>::value, string>::type stringify(const T* val, int size = 1)
{
	string newString = "";
	if (size == 1)	newString = to_string(*val);
	else
	{
		for (int i = 0; i < size; i++)
			newString += to_string(val[i]) + (i < size-1 ? "," : "");
	}
	return newString;
}

template<class T>
typename enable_if<!is_fundamental<T>::value, string>::type stringify(const T* val, int size = 1)
{
	string newString = "";
	if (size == 1)	newString = string(*val);
	else
	{
		for (int i = 0; i < size; i++)
			newString += string(val[i]) + (i < size - 1 ? "," : "");
	}
	return newString;
}

///Selection Sampling algorithm using Algorithm S in 'Art of Computer Programming' Vol.2 by Knuth
template<typename T>
list<T> selection_sample(const T* samples, int sampleCount, int size = 1)
{
	if (sampleCount > size)
		throw new exception("Got Bad sampleSize, needs to be equal or less than the size of the arrayProvided");
	
	int t = 0, m = 0;
	list<T> sampledElements = new list<T>;
	while (m < sampleCount)
	{
		int U = get_random_int(1);
		int sampleIndex = (size - t) * U;
		if (sampleIndex < sampleCount - m)
		{
			sampledElements.push_back(samples[sampleIndex]);
			m++;
		}
		t++;
	}
	return sampledElements;
}

///Shuffling algorithm using Algorithm P in 'Art of Computer Programming' vol.2 by Knuth
///Attributed to Fisher and Yates(1938), Durstenfeld(1964)
template<typename T>
void sample_shuffle(list<T>& samples, int t)
{
	int j = t;
	while (j > 1)
	{
		int U = get_random_int(1);
		int k = ((int)floor(j * U)) + 1;
		swap(samples[j], samples[k]);
		j--;
	}
}

template<typename T>
int get_match_count(list<T>& a, list<T>& b)
{
	int matches = 0;
	if (a.size() != b.size())
		throw new exception("Mismatched list sizes given, need to give equally sized lists");
	for (int i = 0; i < a.size(); i++)
		matches += a[i] == b[i] ? 1 : 0;
	return matches;
}