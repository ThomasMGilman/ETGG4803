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
	
	struct chromosome
	{
		std::vector<int> sequence;
		int matches = 0;

		bool operator> (chromosome& other) const;
		bool operator< (chromosome& other) const;
		bool operator==(chromosome& other) const;
		float operator/(chromosome& other) const;
		float operator/(float other) const;
	};

	MasterMind(const int& size, const int& samples, const int& toKeep);

	~MasterMind();

	chromosome roulette_wheel_selection(vector<chromosome> data)
	{
		utility.quickSort(data, 0, data.size() - 1);
		reverse(data.begin(), data.end() - 1);
		while (true)
		{
			float U = utility.get_random_float();
			for (chromosome val : data)
			{
				if ((val / sizeOfProblem) >= U)
					return val;
			}
		}
	}

	vector<int> create_chromosome_data();

	/// Creates a new chromosome sequence, and checks its matches;
	chromosome create_chromosome();

	/// Create a new population of chromosomes
	vector<chromosome>* populate();

	/// Check to see if mutation chance is reached; if so mutate the chromosomes data
	void mutate_chromosome(chromosome& c, int mutation = -1, bool forceMutation = false);

	/// Check to see if chance for reset is hit; if so reset chromosme to a new sequence
	bool random_reset_check(chromosome& c);

	/// Get a parent chromosome from the most fit parents. Or a reset chromosome if chance for reset hit
	void get_parent(chromosome& child, vector<chromosome>* lastGenParents, int offsetIntoParents);

	vector<chromosome>* create_generation(vector<chromosome>* lastGenParents, bool retainParents);

	void solver(bool retainParents = true);
	
	void print_generation(vector<chromosome>* children);
};


