#pragma once
#include "genetic_algorithm.h"

/*
Best Result of match on Generation 4
toSolveForSize:	10
samples:		1000
parentsToKeep:	10

	//Private chance variables in MasterMind class:
	randomWheelSpinRangeChance = 100;
	randwheelSpinChance = 0;

	randomParentRangeChance = 100;
	randParentChance = 20;

	// Range and chance for reset of rest of population to happen
	resetRangeChance = 100;
	resetChance = 5;

	// Range and chance for mutation to occur
	mutationRangeChance = 100;
	mutationChance = 40;
*/

class MasterMind : GeneticAlgorithm<int>{
protected:
private:
public:
	/// Initializer, takes argument of type T and converts it into a string to solve for

	MasterMind(const int& size, const int& samples, const int& toKeep, vector<int>& sequenceRange) :
		GeneticAlgorithm(size, samples, toKeep, sequenceRange) 
	{
		// Set Mutation Properties
		numBits = 4;	//Nibble 0-15
		maxBitVal = 9;
		minBitVal = 1;

		// Roulette wheel chance
		randomWheelSpinRangeChance = 100;
		randwheelSpinChance = 0;												// 0% works best, as it takes a long time

		// Random Parent chance
		randomParentRangeChance = 100;
		randParentChance = 20;													// 20% works well

		// Range and chance for reset of rest of population to happen
		resetRangeChance = 100;
		resetChance = 5;														// 5% works well

		// Range and chance for mutation to occur
		mutationRangeChance = 100;
		mutationChance = 40;													// 40% works well

		// Set Random Mastermind number range
		*toSolveFor = create_chromosome_data<int>(sequenceValues, size);
		solver();
	};

	void fitness_function_check(chromosome<int>& c) override;
};


