#pragma once
#include <stdafx.h>
#include "genetic_algorithm.h"


class NQueensSolver : GeneticAlgorithm<int>
{
public:
	NQueensSolver(const int& size, const int& samples, int toKeep, vector<int>& sequenceRange, bool debugPrint = false, bool debugPrintBest = false, const int& stagnationPeriodBeforGiveUp = 0) :
		GeneticAlgorithm(size, samples, toKeep, sequenceRange, debugPrint, debugPrintBest, stagnationPeriodBeforGiveUp)
	{
		// Set Mutation Properties
		numBits = 4;	//Nibble 0-15
		maxBitVal = *(sequenceRange.end() - 1);
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

		solver(true, false);
	};

	/////////////////// PARENT CREATION AND GETTER FUNCTIONS //////////////////////////////////////////////////////////////////////

	vector<int> create_chromosome_data() override;

	/////////////////// FITNESS FUNCTION //////////////////////////////////////////////////////////////////////////////////////////

	void fitness_function_check(chromosome<int>& c) override;

	/////////////////// CROSSOVER FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////

	void multi_point_crossover(vector<int>& a, vector<int>& b, int end, int start = NULL) override;

	void uniform_crossover(vector<int>& a, vector<int>& b) override;

	void print_chromosome_data(chromosome<int>& data) override;
};