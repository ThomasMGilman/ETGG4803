#pragma once
#include "genetic_algorithm.h"

/*
Best solution so far: Reaches Answer
Match Distance: 0
Generation:	36715
Time: 22mins

phrase: "To be, or not to be, that is the question."
samples:		1000
parentsToKeep:	10

	numBits = 7;		//0-128
	maxBitVal = 127;
	minBitVal = 32;

	//Private chance variables in MasterMind class:
	randomWheelSpinRangeChance = 100;
	randwheelSpinChance = 0;

	// Random Parent chance
	randomParentRangeChance = 100;
	randParentChance = 60;

	// Range and chance for reset of rest of population to happen
	resetRangeChance = 100;
	resetChance = 30;

	// Range and chance for mutation to occur
	mutationRangeChance = 100;
	mutationChance = 30;
*/

class PhraseSolver : GeneticAlgorithm<char>
{
protected:
private:
public:
	PhraseSolver(const int& size, const int& samples, const int& toKeep, string phrase, vector<char> sequenceRange) :
		GeneticAlgorithm(size, samples, toKeep, sequenceRange, 0)
	{
		for (char c : phrase)
			toSolveFor->push_back(c);

		// Set Mutation Properties
		numBits = 7;		//0-128
		maxBitVal = 127;
		minBitVal = 32;

		// Roulette wheel chance
		randomWheelSpinRangeChance = 100;
		randwheelSpinChance = 0;												// 0% works best, as it takes a long time

		// Random Parent chance
		randomParentRangeChance = 100;
		randParentChance = 60;													// 20% works well

		// Range and chance for reset of rest of population to happen
		resetRangeChance = 100;
		resetChance = 30;														// 5% works well

		// Range and chance for mutation to occur
		mutationRangeChance = 100;
		mutationChance = 50;													// 40% works well

		solver(true, false);
	};

	void fitness_function_check(chromosome<char>& c) override;
};