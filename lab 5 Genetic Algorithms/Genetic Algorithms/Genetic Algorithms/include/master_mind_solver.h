#pragma once
#include <stdafx.h>
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

	MasterMind(vector<int>* toSolveFor, const int& samples, const int& toKeep, vector<int>& sequenceRange, bool debugPrint = false, bool debugPrintBest = false, const int& stagnationPeriodBeforGiveUp = 0) :
		GeneticAlgorithm(0, samples, toKeep, sequenceRange, debugPrint, debugPrintBest, stagnationPeriodBeforGiveUp)
	{
		set_chances_and_bit_values();
		this->toSolveFor = toSolveFor;
		this->sizeOfProblem = toSolveFor->size();
		solver();
	}

	MasterMind(const int& size, const int& samples, const int& toKeep, vector<int>& sequenceRange, bool debugPrint = false, bool debugPrintBest = false, const int& stagnationPeriodBeforGiveUp = 0) :
		GeneticAlgorithm(size, samples, toKeep, sequenceRange, debugPrint, debugPrintBest, stagnationPeriodBeforGiveUp)
	{
		set_chances_and_bit_values();
		*this->toSolveFor = create_chromosome_data(); // Set Random Mastermind number range
		solver();
	};

	void set_chances_and_bit_values();

	void fitness_function_check(chromosome<int>& c) override;
};


