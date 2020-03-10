#pragma once
#include "genetic_algorithm.h"


class NQueensSolver : GeneticAlgorithm<int>
{
public:
	NQueensSolver(const int& size, const int& samples , int toKeep, vector<int>& sequenceRange, int generationsBeforeGiveUp) :
		GeneticAlgorithm(size, samples, toKeep, sequenceRange, generationsBeforeGiveUp) {
		
		// Set Mutation Properties
		numBits = 4;	//Nibble 0-15
		maxBitVal = *(sequenceRange.end()-1);
		minBitVal = *sequenceRange.begin();

		// Roulette wheel chance
		randomWheelSpinRangeChance = 100;
		randwheelSpinChance = 0;												// 0% works best, as it takes a long time

		// Random Parent chance
		randomParentRangeChance = 100;
		randParentChance = 20;													// 20% works well

		// Range and chance for reset of rest of population to happen
		resetRangeChance = 100;
		resetChance = 45;														// 5% works well

		// Range and chance for mutation to occur
		mutationRangeChance = 100;
		mutationChance = 40;													// 40% works well

		solver();
	};

	void fitness_function_check(chromosome<int>& c) override;
};