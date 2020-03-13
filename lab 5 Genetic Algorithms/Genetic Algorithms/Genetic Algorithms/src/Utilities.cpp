#include <stdafx.h>
#include "Utilities.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// UTILITY FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct diceRoll {
	bool evenRoll;
	int even, odd;
	int max, evenleft, oddLeft;
	double probability;
	diceRoll* Even, * Odd;
};
/*
diceRoll setDice(diceRoll* p, bool evenRoll)
{
	diceRoll d;
	//d.eve
}*/

void roll(diceRoll* p)
{
	if (p->evenleft != p->max && p->oddLeft != p->max)
	{
		p->Even->evenRoll = true;
		
	}
}

int get_random_int(int max, int min) {
	lock_guard<mutex> lock(randomMutex);
	int num = (rand() % max + min);
	//cout << "got randomNum: " << to_string(num) << endl;
	return num;
}

float get_random_float(float max, float min)
{
	lock_guard<mutex> lock(randomMutex);
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
			std::cout << "\tPipCount: " << probabilitySamples->first << "\tCount: " << probabilitySamples->second.timesRolled << "\tProbability: " << probabilitySamples->second.probability / probabilitySize << std::endl;
		}
	}
	return pipCount;
}

/*PR(+|ILL) = Sensitivity | PR(+|WELL) = Specificity
PR(ILL|+) = (PR(+|ILL)PR(ILL)) / (PR(+|ILL)PR(ILL) + PR(+|WELL)PR(WELL)
Ex:
	Sensitivity = 99%	Specificity = 99%	TruePositive = 0.5%
	False Positive = 1% = 1-Specificity
	Well = 1 - TruePositive
	PR(ILL|+) = ((.99)*(.005)) / ((.99)*(.005) + (.01)*(.995))
*/
double bayesian_probability(double sensitivity, double specificity, double truePercentage)
{
	return (sensitivity*truePercentage) / (sensitivity * truePercentage +  (1 - specificity) * (1 - truePercentage));
}