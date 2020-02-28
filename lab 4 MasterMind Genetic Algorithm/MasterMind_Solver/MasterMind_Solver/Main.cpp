#include "MasterMind.h"
#include <iostream>
#include <list>
#include <map>
#include <tuple>

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

void main(int argc, char** argv)
{
	const int toSolveForSize = 10;		// number of digits in problem
	const int samples = 1000;			// number of samples per generation
	const int parentsToKeep = 10;		// number of parents to keep in next generation

	MasterMind* mm = new MasterMind(toSolveForSize, samples, parentsToKeep);
	delete(mm);
	std::cout << "done!!" << std::endl;
	//delete(pip_probability_counter(100000, 4, 6, true));
}