#include "Utilities.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// UTILITY FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_random_int(int max, int min) {
	lock_guard<mutex> lock(mtx);
	int num = (rand() % max + min);
	//cout << "got randomNum: " << to_string(num) << endl;
	return num;
}

float get_random_float(float max, float min)
{
	lock_guard<mutex> lock(mtx);
	return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// COUNT FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


std::map<int, PipCount>* pip_probability_counter(int sampleSize, int numDice, int numFaces, bool outputData)
{
	int probabilitySize = pow(numFaces, numDice);
	int sampleSpace = sampleSize;
	std::map<int, PipCount>* pipCount = new std::map<int, PipCount>();
	for (int i = numDice; i <= numDice * numFaces; i++)
		pipCount->emplace(i, PipCount());
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
		for (std::map<int, PipCount>::iterator probabilitySamples = pipCount->begin(); probabilitySamples != pipCount->end(); probabilitySamples++)
		{
			probabilitySamples->second.probability = (float)probabilitySamples->second.timesRolled / (float)sampleSize * probabilitySize;
			std::cout << "\tPipCount: " << probabilitySamples->first << "\tCount: " << probabilitySamples->second.timesRolled << "\tProbability: " << probabilitySamples->second.probability << " / " << probabilitySize << std::endl;
		}
	}
	return pipCount;
}

float bayesian_probability(float sensitivity, float specificity, float truePercentage)
{
	return ((specificity)*truePercentage) / (specificity * truePercentage + specificity * (sensitivity + truePercentage));
}


int check_queens(vector<int>& a)
{
	int invalidSpots = 0;
	bool allZeros = true;
	bool usingZero = false;
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] != 0) allZeros = false;
		if (a[i] == 0)
		{
			usingZero = true;
			invalidSpots += 2;
		}
		bool queenConflict = false;
		for (int j = 0; j < a.size(); j++)
		{
			if (j == i) continue;

			float dRow = abs(a[i] - a[j]);
			float dCol = abs(i - j);
			if (a[i] == a[j]) {
				invalidSpots += pow(a.size(), 2);				// Check in same row
				queenConflict = true;
			}
			if (abs(dRow / dCol) == 1)
			{
				invalidSpots += a.size();				// Check Diagonals
				queenConflict = true;
			}
		}
		if (!queenConflict && !usingZero) invalidSpots++;
	}
	if (allZeros) invalidSpots = pow(a.size(), 2);
	return invalidSpots;
}