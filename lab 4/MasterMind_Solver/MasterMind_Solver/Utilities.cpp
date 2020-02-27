#include "Utilities.h"


int get_random_int(int max, int min)
{
	return rand() % max + min;
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
