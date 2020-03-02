#pragma once
#include "genetic_algorithm.h"

/*
Can probably convert the mastermind class to solve this problem as well.
Or maybe inherit and specialize certain functions for this particular problem.
*/

class PhraseSolver : GeneticAlgorithm<char>
{
protected:
private:
public:
	PhraseSolver(const int& size, const int& samples, const int& toKeep, string phrase, vector<char> sequenceRange) :
		GeneticAlgorithm(size, samples, toKeep, sequenceRange)
	{
		for (char c : phrase)
			toSolveFor->push_back(c);
		solver();
	};

	vector<Utilities::chromosome<char>>* create_generation(vector<Utilities::chromosome<char>>* lastGenParents, bool retainParents);
};