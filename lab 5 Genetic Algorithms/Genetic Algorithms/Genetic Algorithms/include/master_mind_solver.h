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
		*toSolveFor = utility.create_chromosome_data<int>(sequenceValues, size);
		solver();
	};

	vector<Utilities::chromosome<int>>* create_generation(vector<Utilities::chromosome<int>>* lastGenParents, bool retainParents);
};


