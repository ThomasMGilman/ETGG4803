#include "master_mind_solver.h"
#include <iostream>

void main(int argc, char** argv)
{
	
	const int toSolveForSize = 10;		// number of digits in problem
	const int samples = 1000;			// number of samples per generation
	const int parentsToKeep = 10;		// number of parents to keep in next generation
	vector<int> sequenceRange;
	for (int i = 1; i < 10; i++)
		sequenceRange.push_back(i);
	MasterMind* mm = new MasterMind(toSolveForSize, samples, parentsToKeep, sequenceRange);
	delete(mm);
	
	std::cout << "done!!" << std::endl;
}