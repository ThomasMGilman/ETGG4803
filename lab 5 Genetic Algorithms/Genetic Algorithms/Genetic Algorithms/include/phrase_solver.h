#pragma once
#include "Utilities.h"

/*
Can probably convert the mastermind class to solve this problem as well.
Or maybe inherit and specialize certain functions for this particular problem.
*/

class PhraseSolver
{
private:
	Utilities utility;
	vector<char>* phraseToSolveFor;
	vector<char> sequenceValues;
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
protected:
public:
	PhraseSolver(const int& size, const int& samples, const int& toKeep, string phrase, vector<char> sequenceRange);

	~PhraseSolver();

	void get_parent(Utilities::chromosome<char>& c, vector<Utilities::chromosome<char>>* lastGenParents, int offset);

	vector<Utilities::chromosome<int>>* create_generation(vector<Utilities::chromosome<char>>* lastGenParents, bool retainParents);

	void solver(bool retainParents = true);
};