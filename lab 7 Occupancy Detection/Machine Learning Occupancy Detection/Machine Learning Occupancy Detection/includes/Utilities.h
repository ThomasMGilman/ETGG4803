#pragma once
#include <stdafx.h>
#include <sorting_functions.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// UTILITY STRUCTS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void swap_data(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
};

template<typename T>
vector<T>concat_vectors(vector<T> vecA, vector<T> vecB)
{
	vector<T> AB;
	AB.reserve(vecA.size() + vecB.size());
	AB.insert(AB.end(), vecA.begin(), vecA.end());
	AB.insert(AB.end(), vecB.begin(), vecB.end());
	return AB;
}

struct PipCount
{
	int timesRolled;
	float probability;
};

template<typename T>
struct chromosome
{
	vector<T> sequence;
	int matches = -1;
	int crossOverChosen = -1;
	int mutationChosen = -1;

	bool operator> (chromosome<T>& other) const { return matches > other.matches; };
	bool operator< (chromosome<T>& other) const { return matches < other.matches; };
	bool operator==(chromosome<T>& other) const 
	{ 
		for (int i = 0; i < sequence.size(); i++)
			if (sequence[i] != other.sequence[i]) return false;
		return true; 
	};
	bool operator!=(chromosome<T>& other) const { return !(*this == other); };
	float operator/(chromosome<T>& other) const { return (float)matches / (float)other.matches; };
	float operator/(float other) const { return (float)matches / other; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// RANDOM FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_random_int(int max = 1, int min = 0);

float get_random_float(float max = 1.0f, float min = 0.0f);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// CONVERSION FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///Returns the string version of the value passed in
template<class T>
typename enable_if<is_fundamental<T>::value, string>::type stringify(T val)
{
	return to_string(val);
}

///Returns the string version of the vector of type T if type T is able to be processed by std::to_string
template<class T>
typename enable_if<is_fundamental<T>::value, string>::type stringify(vector<T> val)
{
	string newString = "";
	unsigned size = val.size();
	for (unsigned i = 0; i < size; i++)
		newString += to_string(val[i]) + (i < size - 1 ? "," : "");
	return newString;
}

///Returns the string version of the vector of type T if type T is able to be casted to type string
template<class T>
typename enable_if<!is_fundamental<T>::value, string>::type stringify(vector<T> val)
{
	string newString = "";
	unsigned size = val.size();
	for (unsigned i = 0; i < size; i++)
		newString += (string)((void)(val[i])) + (i < size - 1 ? "," : "");
	return newString;
}

/// Converts Vector of Data of type T to a string using std::to_string
template<typename T>
string convert_to_string(vector<T>& data)
{
	string newString = "";
	for (int i = 0; i < data.size(); i++)
		newString += to_string(data[i]) + (i < data.size() - 1 ? "," : "");
	return newString;
};

/// Converts Vector of Data of type T to a string using std::stringstream
template<typename T>
string convert_to_string_using_stringstream(vector<T>& data)
{
	stringstream ss;
	for (int i = 0; i < data.size(); i++)
		ss << data[i];
	return ss.str();
}

/// Converts Data of type T to a string using std::stringstream
template<typename T>
string convert_data_to_string_using_stringstream(T& data)
{
	stringstream ss;
	ss << data;
	return ss.str();
}

/// Converts vector of strings to vector of ints, if contained string isnt a numerical value, returns 0 for that string
vector<int> convert_strings_to_int(vector<string> strings);

/// Converts vector of strings to vector of floats, if contained string isnt a numerical value, returns 0 for that string
vector<float> convert_strings_to_float(vector<string> strings);

/// Converts vector of strings to vector of doubles, if contained string isnt a numerical value, returns 0 for that string
vector<double> convert_strings_to_double(vector<string> strings);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// STRING FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string create_time_string(int Hour, int Minute, int Second);

string create_date_string(int Year, int Month, int Day);

/// Retrieve all the sub strings between the delimeter characters
vector<string> get_sub_strings(string line, unordered_set<char> attrib_delim, unordered_set<char> to_ignore);

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
/////////////////// STATISTICAL FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Get the probability of rolling a number with 'N' number of fair dice with 'F' faces out of 'R' number of rolls
std::map<int, PipCount>* pip_probability_counter(int sampleSize, int numDice, int numFaces, bool outputData = false);

float bayesian_probability(float sensitivity, float specificity, float testPositive);

double bayesian_probability(double sensitivity, double specificity, double testPositive);

///Get the number of matches between a and b
template<typename T>
int get_match_count(vector<T>& a, vector<T>& b)
{
	int matches = 0;
	if (a.size() != b.size())
		throw new exception("Mismatched list sizes given, need to give equally sized lists");
	for (int i = 0; i < a.size(); i++)
		matches += a[i] == b[i] ? 1 : 0;
	return matches;
};

template<typename T>
T normalize_var(T var, T Min, T Range)
{
	return (var - Min) / Range;
}

/// Calculate The total difference count between the two vector values
template<typename T>
int get_match_difference_offset(vector<T>& a, vector<T>& b)
{
	int distance = 0;
	if (a.size() != b.size())
		throw new exception("Mismatched list sizes given, need to give equally sized lists");
	for (int i = 0; i < a.size(); i++)
		distance += abs(a[i] - b[i]);
	return distance;
}

/// Calculate The mean of a vector 
template<typename T>
T calculate_mean(vector<T>& values)
{
	auto type_hash = typeid(T).hash_code();
	if (type_hash == typeid(int).hash_code() || type_hash == typeid(float).hash_code() || type_hash == typeid(double).hash_code())
	{
		T total = 0;
		for (T val : values)
			total += val;
		return total / values.size();
	}
	else
	{
		string typeName = typeid(T).name();
		throw new exception(("Unsupported Type Error!!! Cannot calculate the mean of a vector of type: " + typeName).c_str());
	}
}

/// Calculate the standard deviation of the values provided given the mean
template<typename T>
T calculate_standard_deviation(vector<T>& values, T mean)
{
	auto type_hash = typeid(T).hash_code();
	if (type_hash == typeid(int).hash_code() || type_hash == typeid(float).hash_code() || type_hash == typeid(double).hash_code())
	{
		T deviation_amnt = 0;
		for (int i = 0; i < values.size(); i++)
			deviation_amnt += pow(values[i] - mean, 2);
		return sqrt(deviation_amnt / (values.size() - 1));
	}
	else
	{
		string typeName = typeid(T).name();
		throw new exception(("Unsupported Type Error!!! Cannot calculate the deviation of a vector of type: " + typeName).c_str());
	}
}

/// Calculate Gaussian probability distribution for Val, must provide the mean and standard deviation
template<typename T>
double calculate_gaussian_probab_density(T val, T mean, T standard_deviation)
{
	double exponent = exp(-(pow(val - mean, 2) / (2 * pow(standard_deviation, 2))));
	return (1 / (sqrt(2 * M_PI) * standard_deviation)) * exponent;
}

/*Retrieves the middle value from a number vector given!! if the list is not sorted, the function sorts the list then retrieves the value.
The user has the option to allow the program to sort the list passed. By default the function makes a copy then sorts the list.*/
template<typename T>
double get_median(vector<T>& values, bool list_sorted, bool sort_passed_list = false)
{
	auto type_hash = typeid(T).hash_code();
	if (type_hash == typeid(int).hash_code() || type_hash == typeid(float).hash_code() || type_hash == typeid(double).hash_code())
	{
		vector<T>* list_picker = values;
		if (!list_sorted)
		{
			if (sort_passed_list)
				quickSort<T>(values, 0, values.size() - 1);
			else
			{
				vector<T> copy = values;
				quickSort<T>(copy, 0, copy.size() - 1);
				list_picker = copy;
			}
		}
		return list_picker[unsigned(values.size() / 2)];
	}
	else
	{
		string typeName = typeid(T).name();
		throw new exception(("Unsupported Type Error!!! Cannot get the median of a vector of type: " + typeName).c_str());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// TIME FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// SELECTION FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Evaluate fitness score for each chromosome
Normalize all fitness scores
Sort normalized scores in descending order
Generate random number U, uniformly distributed between 0 and 1
Select first chromosome whose accumulated normalized fitness score is at least U
Repeat to select second parent
Repeat parent selection process as many times as desired
*/
template<typename T>
T roulette_wheel_selection(vector<T>& data, int maxMatches, bool reverseOrder = true)
{
	while (true)
	{
		float U = get_random_float();
		float accumulatedScore = 0;
		if(reverseOrder)
		{
			for (int i = data.size() - 1; i > 0; i--)
			{
				accumulatedScore += (data[i] / maxMatches);
				if (accumulatedScore >= U)
					return safe_copy_data_from_vector(data, i);
			}
		}
		else
		{
			for (int i = 0; i < data.size(); i++)
			{
				accumulatedScore += (data[i] / maxMatches);
				if (accumulatedScore >= U)
					return safe_copy_data_from_vector(data, i);
			}
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
/////////////////// COPY FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
T safe_copy_data_from_vector(vector<T>& from, int index)
{
	T data = from.at(index);
	return data;
}

template<typename T>
void safe_copy_data_from_vector(T& data, vector<T>& from, int index)
{
	data = from.at(index);
}

template<typename T>
void safe_copy_vector(vector<T>& copyTo, vector<T>& copyFrom)
{
	copyTo = copyFrom;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// FILE FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Appends the specified file to the specified path and returns the result
string append_file_to_path(string file_path, string file_name);

/// Returns true if file exists in specified directory otherwise false
bool file_exists(string file_path, string file_name);

/// Returns a vector of all files with full path if path passed exists and is a directory.
/// Otherwise an empty vector is returned
vector<string> get_files_in_directory(string dir_path);

/// Reads every line from a file into a vector container and returns the container back to the caller.
/// Throws an error if a file exception occurs or the file cannot be found.
vector<string> file_reader(string file_path, string file_name);

/// write the lines to the file specified the close it.
int file_writer(vector<string> lines, string file_path, string file_name, bool write_at_end = true, bool clear_file = false);

/// write the string to the file specified then close it.
int file_writer(string line, string file_path, string file_name, bool write_at_end = true, bool clear_file = false);

/// write the string to the file if open.
void file_writer(string line, ofstream* file, bool add_new_line = true, bool close_after_write = false);