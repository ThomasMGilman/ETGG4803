#pragma once
#include "Utilities.h"

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

class MasterMind {
protected:
private:
	Utilities utility;
	vector<int>* toSolveFor;
	vector<int> sequenceValues;
	int sizeOfProblem;
	int samplesPerGeneration;
	int parentsToKeep;
	int generation = 0;

	// Roulette wheel chance
	int randomWheelSpinRangeChance = 100;
	int randwheelSpinChance = 0;									// 0% works best, as it takes a long time

	// Random Parent chance
	int randomParentRangeChance = 100;
	int randParentChance = 20;										// 20% works well

	// Range and chance for reset of rest of population to happen
	int resetRangeChance = 100;
	int resetChance = 5;											// 5% works well

	// Range and chance for mutation to occur
	int mutationRangeChance = 100;
	int mutationChance = 40;										// 40% works well

public:
	/// Initializer, takes argument of type T and converts it into a string to solve for

	MasterMind(const int& size, const int& samples, const int& toKeep, vector<int>& sequenceRange);

	~MasterMind();

	/// Check to see if mutation chance is reached; if so mutate the chromosomes data
	void mutate_chromosome(Utilities::chromosome<int>& c, int mutation = -1, bool forceMutation = false);

	void get_parent(Utilities::chromosome<int>& c, vector<Utilities::chromosome<int>>* lastGenParents, int offset);

	vector<Utilities::chromosome<int>>* create_generation(vector<Utilities::chromosome<int>>* lastGenParents, bool retainParents);

	void solver(bool retainParents = true);
};


