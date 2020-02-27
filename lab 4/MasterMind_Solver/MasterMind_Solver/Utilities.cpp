#include "Utilities.h"
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// UTILITY STRUCTS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void Utilities::swap_data(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

int Utilities::get_random_int(int max, int min)
{
	return (rand() % max + min);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// STRING FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Utilities::convert_to_string(vector<int>& data)
{
	string newString = "";
	for (int i = 0; i < data.size(); i++)
		newString += to_string(data[i]) + (i < data.size() - 1 ? "," : "");
	return newString;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// COUNT FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<int, Utilities::PipCount>* Utilities::pip_probability_counter(int sampleSize, int numDice, int numFaces, bool outputData)
{
	int probabilitySize = pow(numFaces, numDice);
	int sampleSpace = sampleSize;
	std::map<int, Utilities::PipCount>* pipCount = new std::map<int, Utilities::PipCount>();
	for (int i = numDice; i <= numDice * numFaces; i++)
		pipCount->emplace(i, Utilities::PipCount());
	do
	{
		int num = 0;
		for (int i = 0; i < numDice; i++)
			num += get_random_int(6, 1);

		pipCount->at(num).timesRolled += 1;
	} while (sampleSpace--);

	if (outputData)
	{
		std::cout << "standard fair " << numFaces << "-Sided dice with " << numDice << " dice samples and their probability\n\tMaxChance: " << probabilitySize << " SampleSize: " << sampleSize << "\n" << std::endl;
		for (std::map<int, Utilities::PipCount>::iterator probabilitySamples = pipCount->begin(); probabilitySamples != pipCount->end(); probabilitySamples++)
		{
			probabilitySamples->second.probability = (float)probabilitySamples->second.timesRolled / (float)sampleSize * probabilitySize;
			std::cout << "\tPipCount: " << probabilitySamples->first << "\tCount: " << probabilitySamples->second.timesRolled << "\tProbability: " << probabilitySamples->second.probability << " / " << probabilitySize << std::endl;
		}
	}
	return pipCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////// MUTATION FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///Shuffling algorithm using Algorithm P in 'Art of Computer Programming' vol.2 by Knuth
	///Attributed to Fisher and Yates(1938), Durstenfeld(1964)
void Utilities::sample_shuffle(vector<int>& samples, int t)
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
void Utilities::scramble(vector<int>& samples)
{
	random_shuffle(samples.begin(), samples.end());
};

/// Invert all numbers to their bit value within the given range within the maxnumber of bits
void Utilities::bit_inversion(vector<int>& numbers, int numbits, int maxrange, int minrange)
{
	int bitRange = pow(2, numbits) - 1;
	if (maxrange > bitRange) maxrange = bitRange;
	int dif = bitRange - maxrange;
	for (int i : numbers)
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
void Utilities::multi_point_crossover(vector<int>& a, vector<int>& b, int end, int start)
{
	if (a.size() != b.size())
		throw new exception("passed mismatched vector sizes!!!");
	else if (end >= a.size() || end < 0 || (start != NULL && (start >= a.size() || start < 0 || start > end)))
		throw new exception("Given an crossover point greater than or less than size of vectors");

	int begining = start != NULL ? start : 0;
	for (int i = begining; i <= end; i++)
		swap_data(a.at(i), b.at(i));
};

vector<int> Utilities::uniform_crossover(vector<int>& a, vector<int>& b)
{
	vector<int> child;
	if (a.size() != b.size())
		throw new exception("passed mismatched vector sizes!!!");

	for (int i = 0; i < a.size(); i++)
		child.push_back(rand() % 2 ? a.at(i) : b.at(i));
	return child;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// SAMPLING FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///Selection Sampling algorithm using Algorithm S in 'Art of Computer Programming' Vol.2 by Knuth
vector<int> Utilities::selection_sample(vector<int>& samples, int sampleCount)
{
	if (sampleCount > samples.size())
		throw new exception("Got Bad sampleSize, needs to be equal or less than the size of the arrayProvided");

	int t = 0, m = 0;
	vector<int> sampledElements;
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