#pragma once
#include "Utilities.h"


class NQueensSolver
{
private:
	int boardSize;
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
	NQueensSolver(int boardSize, int samplesPerGen, int parentsToKeep, int generationsBeforeGiveUp);

};