#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <thread>
#include <mutex>
#include <time.h>
#include <chrono>

using namespace std;

static mutex mtx;	//global mutex lock

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// UTILITY STRUCTS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PipCount
{
	int timesRolled;
	float probability;
};

template<typename T>
struct chromosome
{
	std::vector<T> sequence;
	int matches = 0;

	bool operator> (chromosome<T>& other) const { return matches > other.matches; };
	bool operator< (chromosome<T>& other) const { return matches < other.matches; };
	bool operator==(chromosome<T>& other) const { return matches == other.matches; };
	float operator/(chromosome<T>& other) const { return (float)matches / (float)other.matches; };
	float operator/(float other) const { return (float)matches / other; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// UTILITY FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void swap_data(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
};

int get_random_int(int max = 1, int min = 0);

float get_random_float(float max = 1.0f, float min = 0.0f);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// STRING FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///Returns the string version of the value passed in
template<class T>
typename enable_if<is_fundamental<T>::value, string>::type stringify(T& val, int size = 1)
{
	string newString = "";
	if (size == 1)	newString = to_string(val);
	else
	{
		for (int i = 0; i < size; i++)
			newString += to_string(val[i]) + (i < size - 1 ? "," : "");
	}
	return newString;
}

template<class T>
typename enable_if<!is_fundamental<T>::value, string>::type stringify(T& val, int size = 1)
{
	string newString = "";
	if (size == 1)	newString = string(val[0]);
	else
	{
		for (int i = 0; i < size; i++)
			newString += (string)((void)(val[i])) + (i < size - 1 ? "," : "");
	}
	return newString;
}

template<typename T>
string convert_to_string(vector<T>& data)
{
	string newString = "";
	for (int i = 0; i < data.size(); i++)
		newString += to_string(data[i]) + (i < data.size() - 1 ? "," : "");
	return newString;
};

template<typename T>
string convert_to_string_using_stringstream(vector<T>& data)
{
	stringstream ss;
	for (int i = 0; i < data.size(); i++)
		ss << data[i];
	return ss.str();
}

template<typename T>
string convert_data_to_string_using_stringstream(T& data)
{
	stringstream ss;
	ss << data;
	return ss.str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// DEBUG FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void print_dif_amount(vector<T>& a, vector<T>& b)
{
	if (a.size() != b.size())
		throw new exception("Invalid vector sizes given!!");
	cout << "a\t" << "diff\t" << "b" << endl;
	for (int i = 0; i < a.size(); i++)
	{
		cout << convert_data_to_string_using_stringstream<T>(a[i]) << "\t" <<
			abs(a[i] - b[i]) << "\t" <<
			convert_data_to_string_using_stringstream<T>(b[i]) << endl;
	}
	cout << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// COUNT FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<int, PipCount>* pip_probability_counter(int sampleSize, int numDice, int numFaces, bool outputData = false);

float bayesian_probability(float sensitivity, float specificity, float testPositive);

///Get the number of matches between a and b
template<typename T>
int get_match_count(vector<T>& a, vector<T>& b)
{
	lock_guard<mutex> lock(mtx);
	int matches = 0;
	if (a.size() != b.size())
		throw new exception("Mismatched list sizes given, need to give equally sized lists");
	for (int i = 0; i < a.size(); i++)
		matches += a[i] == b[i] ? 1 : 0;
	return matches;
};

/// Calculate The total difference count between the two vector values
template<typename T>
int get_match_difference_offset(vector<T>& a, vector<T>& b)
{
	lock_guard<mutex> lock(mtx);
	int distance = 0;
	if (a.size() != b.size())
		throw new exception("Mismatched list sizes given, need to give equally sized lists");
	for (int i = 0; i < a.size(); i++)
		distance += abs(a[i] - b[i]);
	return distance;
}

int check_queens(vector<int>& a);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// TIME FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// SELECTION FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
T roulette_wheel_selection(vector<T> data, int maxMatches)
{
	quickSort(data, 0, data.size() - 1);
	reverse(data.begin(), data.end() - 1);
	while (true)
	{
		float U = get_random_float();
		for (T val : data)
		{
			if ((val / maxMatches) >= U)
				return val;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// MUTATION FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///Shuffling algorithm using Algorithm P in 'Art of Computer Programming' vol.2 by Knuth
///Attributed to Fisher and Yates(1938), Durstenfeld(1964)
template<typename T>
void sample_shuffle(vector<T>& samples, int t)
{
	int j = t;
	while (j > 1)
	{
		int U = get_random_int(1);
		int k = ((int)floor(j * U)) + 1;
		swap_data(samples[j], samples[k]);
		j--;
	}
};

///random shuffle of samples
template<typename T>
void scramble(vector<T>& samples)
{
	random_shuffle(samples.begin(), samples.end());
};

/// Invert all numbers to their bit value within the given range within the maxnumber of bits
template<typename T>
void bit_inversion(vector<T>& data, int numbits, T maxrange, T minrange)
{
	int bitRange = pow(2, numbits) - 1;
	if (maxrange > bitRange) maxrange = bitRange;
	int dif = bitRange - maxrange;
	for (T i : data)
	{
		i = ~i & bitRange;						// invert bits and floor number to 4bit range
		if (i > maxrange) i -= dif;				// keep numbers in range of min to max
		else if (i < minrange) i = minrange;	// Should never happen
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// SAMPLING FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///Selection Sampling algorithm using Algorithm S in 'Art of Computer Programming' Vol.2 by Knuth
template<typename T>
vector<T> selection_sample(vector<T>& samples, int sampleCount)
{
	if (sampleCount > samples.size())
		throw new exception("Got Bad sampleSize, needs to be equal or less than the size of the arrayProvided");

	int t = 0, m = 0;
	vector<T> sampledElements;
	while (m < sampleCount)
	{
		int U = get_random_int(1);
		int sampleIndex = (samples.size() - t) * U;
		if (sampleIndex < sampleCount - m)
		{
			sampledElements.push_back(samples[sampleIndex]);
			m++;
		}
		t++;
	}
	return sampledElements;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// SORTING FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Determins the pivot index of the incoming data between start and end indexs'
/// Swaps data to either side of pivot based on less than or greater than said value
template<typename T>
int partition(vector<T>& data, int start, int end)
{
	T pivot = data[end];
	int i = (start - 1);
	for (int j = start; j <= end - 1; j++)
	{
		if (data[j] < pivot)
		{
			i++;
			swap(data[i], data[j]);
		}
	}
	swap(data[i + 1], data[end]);
	return (i + 1);
};

/// Recursively swaps data from either side of the pivot based on if data is greater or lass than pivot value
template<typename T>
void quickSort(vector<T>& data, int start, int end)
{
	if (start < end)
	{
		int pi = partition(data, start, end);
		quickSort(data, start, pi - 1);
		quickSort(data, pi + 1, end);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// COPY FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
T safe_copy_data_from_vector(vector<T>& from, int index)
{
	lock_guard<mutex> lock(mtx);
	T data = from.at(index);
	return data;
}

template<typename T>
void safe_copy_data_from_vector(T& data, vector<T>& from, int index)
{
	lock_guard<mutex> lock(mtx);
	data = from.at(index);
}

template<typename T>
void safe_copy_vector(vector<T>& copyTo, vector<T>& copyFrom)
{
	lock_guard<mutex> lock(mtx);
	copyTo = copyFrom;
}