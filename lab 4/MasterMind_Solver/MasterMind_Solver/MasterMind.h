#pragma once
#include "Utilities.h"
#include <vector>

class MasterMind {
protected:
private:
	vector<int>* toSolveFor;
	int samplesPerGeneration;
	int parentsToKeep;
	int generation = 0;
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

	void inversion(chromosome c);

	chromosome create_chromosome();	

	vector<chromosome>* populate();

	void solver(vector<chromosome>* lastGenParents = nullptr);
};


