#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

struct PipCount
{
	int timesRolled;
	float probability;
};

int get_random_int(int max, int min = 0);

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

std::map<int, PipCount>* pip_probability_counter(int sampleSize, int numDice, int numFaces, bool outputData = false);

///Selection Sampling algorithm using Algorithm S in 'Art of Computer Programming' Vol.2 by Knuth
template<typename T>
vector<T> selection_sample(vector<T>& samples, int sampleCount);

///Shuffling algorithm using Algorithm P in 'Art of Computer Programming' vol.2 by Knuth
///Attributed to Fisher and Yates(1938), Durstenfeld(1964)
template<typename T>
void sample_shuffle(vector<T>& samples, int t);

///random shuffle of samples
template<typename T>
void scramble(vector<T>& samples);

/// Invert all numbers to their bit value within the given range within the maxnumber of bits
void inversion(vector<int>& numbers, int numbits, int maxrange, int minrange);

///Get the number of matches between a and b
template<typename T>
int get_match_count(vector<T>& a, vector<T>& b);

///sawp the contents of a and b up to the point of crossover
template<typename T>
void multi_point_crossover(vector<T>& a, vector<T>& b, int start, int end = NULL);