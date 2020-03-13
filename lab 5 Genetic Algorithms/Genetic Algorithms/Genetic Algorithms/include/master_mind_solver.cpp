#include <stdafx.h>
#include "master_mind_solver.h"

void MasterMind::set_chances_and_bit_values()
{
	// Set Mutation Properties
	numBits = 4;	//Nibble 0-15
	maxBitVal = 9;
	minBitVal = 1;

	// Roulette wheel chance
	randomWheelSpinRangeChance = 100;
	randwheelSpinChance = 5;												// 0% works best, as it takes a long time

	// Random Parent chance
	randomParentRangeChance = 100;
	randParentChance = 20;													// 20% works well

	// Range and chance for reset of rest of population to happen
	resetRangeChance = 100;
	resetChance = 5;														// 5% works well

	// Range and chance for mutation to occur
	mutationRangeChance = 100;
	mutationChance = 40;													// 40% works well
}

void MasterMind::fitness_function_check(chromosome<int>& c)
{
	c.matches = get_match_count(c.sequence, *toSolveFor);
}
