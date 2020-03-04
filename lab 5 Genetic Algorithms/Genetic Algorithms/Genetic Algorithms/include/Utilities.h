#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <thread>
#include <mutex>

using namespace std;

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

int get_random_int(mutex& mtx, int max, int min = 0);

float get_random_float(mutex& mtx, float max = 1.0f, float min = 0.0f);

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
string convert_to_string2(vector<T>& data)
{
	stringstream ss;
	for (int i = 0; i < data.size(); i++)
		ss << data[i];
	return ss.str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// DEBUG FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void print_chromosome_data(chromosome<T>& data)
{
	cout << "matches: " << data.matches << " child:\t" << convert_to_string2(data.sequence) << endl;
}

template<typename T>
void print_chromosome_vec(vector<chromosome<T>>& data, int generation)
{
	cout << "Generation: " << generation << endl;
	for (chromosome<T> c : data)
		print_chromosome_data<T>(c);
	cout << "\n" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// COUNT FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<int, PipCount>* pip_probability_counter(int sampleSize, int numDice, int numFaces, bool outputData = false);

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
void scramble(mutex& mtx,vector<T>& samples)
{
	mtx.lock();
	random_shuffle(samples.begin(), samples.end());
	mtx.unlock();
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
/////////////////// CROSSOVER FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///sawp the contents of a and b up to the point of crossover
///should pass the children/copies of the parent vectors to this function to retain parent info
template<typename T>
void multi_point_crossover(vector<T>& a, vector<T>& b, int end, int start = NULL)
{
	if (a.size() != b.size())
		throw new exception("passed mismatched vector sizes!!!");
	else if (end >= a.size() || end < 0 || (start != NULL && (start >= a.size() || start < 0 || start > end)))
		throw new exception("Given an crossover point greater than or less than size of vectors");

	int begining = start != NULL ? start : 0;
	for (int i = begining; i <= end; i++)
		swap_data(a.at(i), b.at(i));
};

template<typename T>
vector<T> uniform_crossover(mutex& mtx, vector<T>& a, vector<T>& b)
{
	vector<T> child;
	if (a.size() != b.size())
		throw new exception("passed mismatched vector sizes!!!");

	for (int i = 0; i < a.size(); i++)
		child.push_back(get_random_int(mtx, 2) ? a.at(i) : b.at(i));
	return child;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// SAMPLING FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///Selection Sampling algorithm using Algorithm S in 'Art of Computer Programming' Vol.2 by Knuth
template<typename T>
vector<T> selection_sample(mutex& mtx, vector<T>& samples, int sampleCount)
{
	if (sampleCount > samples.size())
		throw new exception("Got Bad sampleSize, needs to be equal or less than the size of the arrayProvided");

	int t = 0, m = 0;
	vector<T> sampledElements;
	while (m < sampleCount)
	{
		int U = get_random_int(mtx, 1);
		int sampleIndex = (samples.size() - t) * U;
		if (sampleIndex < sampleCount - m)
		{
			sampledElements.push_back(samples[sampleIndex]);
			m++;
		}
		t++;
	}
	return sampledElements;
};;

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
/////////////////// GENETIC FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
vector<T> create_chromosome_data(mutex& mtx, vector<T>& sequenceToCreateFrom, int sizeOfChromosomeData)
{
	vector<T> sequence;
	for (int i = 0; i < sizeOfChromosomeData; i++)
		sequence.push_back(sequenceToCreateFrom.at(get_random_int(mtx, sequenceToCreateFrom.size())));
	return sequence;
}

template<typename T>
chromosome<T> create_chromosome(mutex& mtx, vector<T>& sequenceToCreateFrom, vector<T>& sequenceToMatch, int (*fitnessFunc)(vector<T>&, vector<T>&))
{
	chromosome<T> c;
	c.sequence = create_chromosome_data<T>(mtx, sequenceToCreateFrom, sequenceToMatch.size());
	c.matches = fitnessFunc(c.sequence, sequenceToMatch);
	return c;
}

template<typename T>
vector<chromosome<T>>* create_population(mutex& mtx,vector<T>& sequenceToCreateFrom, vector<T>& sequenceToMatch, int populationSize, int (*fitnessFunc)(vector<T>&, vector<T>&))
{
	vector<chromosome<T>>* population = new vector<chromosome<T>>();
	for (int i = 0; i < populationSize; i++)
		population->push_back(create_chromosome<T>(mtx, sequenceToCreateFrom, sequenceToMatch, fitnessFunc));
	return population;
}

template<typename T>
bool random_reset_check(mutex& mtx, chromosome<T>& c, vector<T>& sequenceToMatch, vector<T>& sequenceToCreateFrom, int randResetChanceRange, int randResetChance, int (*fitnessFunc)(vector<T>&, vector<T>&))
{
	int chance = rand() % randResetChanceRange + 1;
	if (chance <= randResetChance)
	{
		c = create_chromosome<T>(mtx, sequenceToCreateFrom, sequenceToMatch, fitnessFunc);
		return true;
	}
	return false;
}

template<typename T>
void mutate_chromosome(mutex& mtx, chromosome<T>& c, int mutationChanceRange, int mutationChance,
	int numbits, int maxRange, int minRange,
	int mutation = -1, bool forceMutation = false)
{
	int chance = get_random_int(mtx, mutationChanceRange, 1);
	if (chance <= mutationChance || forceMutation)
	{
		int m = mutation != -1 ? mutation : get_random_int(mtx, 3);
		switch (m)
		{
		case 0:	//Shuffle the childs data
		{
			sample_shuffle<T>(c.sequence, get_random_int(mtx, c.sequence.size()));
			break;
		}
		case 1:	//Scramble the childs data
		{
			scramble<T>(c.sequence);
			break;
		}
		case 2:	//invert the childs bit data
		{
			bit_inversion<T>(c.sequence, numbits, maxRange, minRange);
			break;
		}
		}
	}
}

template<typename T>
void get_parent(chromosome<T>& parent, vector<T>& sequenceToMatch, vector<chromosome<T>>* lastGenParents, vector<T>& sequenceToCreateFrom,
	int parentsToKeep, int offsetIntoParents,
	int randParChanceRange, int randParChance,
	int randWheelSpinChanceRange, int randWheelSpinChance,
	int randResetChanceRange, int randResetChance,
	mutex& mtx,
	int (*fitnessFunc)(vector<T>&, vector<T>&))
{
	int index, chance = get_random_int(randParChanceRange + 1);
	if (chance <= randParChance)
	{
		index = rand() % lastGenParents->size();
		parent = lastGenParents->at(index);
	}
	else if (!random_reset_check<T>(parent, sequenceToMatch, sequenceToCreateFrom, randResetChanceRange, randResetChance, fitnessFunc))
	{
		chance = rand() % randWheelSpinChanceRange + 1;
		if (chance <= randWheelSpinChance)
		{
			vector<chromosome<T>> copy = *lastGenParents;
			parent = roulette_wheel_selection<chromosome<T>>(copy, sequenceToMatch.size());
		}
		else
		{
			index = rand() % parentsToKeep + offsetIntoParents;
			parent = lastGenParents->at(index);
		}
	}
}