#pragma once
#include "Utilities.h"
#include <vector>

class MasterMind {
protected:
private:
	Utilities utility;
	vector<int>* toSolveFor;
	int sizeOfProblem;
	int samplesPerGeneration;
	int parentsToKeep;
	int generation = 0;
	int resetRangeChance = 100;
	int resetChance = 99;
public:
	/// Initializer, takes argument of type T and converts it into a string to solve for
	
	struct chromosome
	{
		std::vector<int> sequence;
		int matches = 0;

		bool operator> (chromosome& other) const;
		bool operator< (chromosome& other) const;
		bool operator==(chromosome& other) const;
	};

	MasterMind(const int& size, const int& samples, const int& toKeep);

	~MasterMind();

	vector<int> create_chromosome_data();

	chromosome create_chromosome();

	vector<chromosome>* populate();

	vector<chromosome>* create_generation(vector<chromosome>* lastGenParents, bool retainParents);

	void solver(bool retainParents = true);
	
	void print_generation(vector<chromosome>* children);
};


