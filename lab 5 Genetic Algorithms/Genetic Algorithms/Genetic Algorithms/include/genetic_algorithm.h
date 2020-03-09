#pragma once
#include "Utilities.h"

template<typename T>
class GeneticAlgorithm
{
private:
protected:
	vector<T>* toSolveFor;
	vector<T> sequenceValues;
	int samplesPerGeneration;
	int parentsToKeep;
	int sizeOfProblem;
	int SPBGU;
	int lastBestGen = 0;
	int generation = 0;

	int bestMatchCount;

	// Mutation propertys
	int numBits;
	int maxBitVal;
	int minBitVal;

	// Roulette wheel chance
	int randomWheelSpinRangeChance;
	int randwheelSpinChance;

	// Random Parent chance
	int randomParentRangeChance;
	int randParentChance;

	// Range and chance for reset of rest of population to happen
	int resetRangeChance;
	int resetChance;

	// Range and chance for mutation to occur
	int mutationRangeChance;
	int mutationChance;

	//Fitness Function pointer
	int (*fitnessFunc)(vector<T>&, vector<T>&);

	// Timer
	chrono::steady_clock::time_point start, end;

public:
	GeneticAlgorithm(const int& size, const int& samples, const int& parentsToKeep, vector<T> sequenceRange, const int& stagnationPeriodBeforGiveUp = 0, int (*fitFunc)(vector<T>&, vector<T>&) = get_match_count<T>);

	~GeneticAlgorithm();

	void get_gen_parent(chromosome<T>& c, vector<chromosome<T>>* lastGenParents, int offset);

	void mutate_chromosome(chromosome<T>& c, int mutationChanceRange, int mutationChance, int numbits, int maxRange, int minRange, int mutation = -1, bool forceMutation = false);

	virtual void pick_cross_over(vector<chromosome<T>>* lastGenParents, vector<chromosome<T>>* newGen, int offset, int numChildrenToPush, int crossOverChosen);

	/*
	Create a new generation using last gen parents. If specified, will retain a requested number of parents to keep in the next generation until a better parent is found.
	Creates children through crossover functions, random or forced mutation, or even a random reset if resets are allowed.
	Upon creation of a new child, it is matched with the problem to solve for, to provide a fitness value to the child.

	ARGUMENTS
	lastGenParents	: Vector of last generations parents to use for the crossover functions
	retainParents	: Keep a fixed number of best parents in the next generation until a better one is found
	higherFitness	: Specifies whether to use a higher or lower fitness score for best parent.
	*/
	virtual vector<chromosome<T>>* create_generation(vector<chromosome<T>>* lastGenParents, bool retainParents, bool higherFitness);

	virtual void solver(bool retainParents = true, bool higherFitness = true);

	void printBest(chromosome<T>* best);

	void debug_check_print_best_chromosome(vector<chromosome<T>>& data, bool higherFitness, bool printBest = true);
};

template<typename T>
inline GeneticAlgorithm<T>::GeneticAlgorithm(const int& size, const int& samples, const int& toKeep, vector<T> sequenceRange, const int& stagnationPeriodBeforGiveUp, int (*fitFunc)(vector<T>&, vector<T>&))
{
	toSolveFor = new vector<T>();
	SPBGU = stagnationPeriodBeforGiveUp;
	sequenceValues = sequenceRange;
	sizeOfProblem = size;
	samplesPerGeneration = samples;
	parentsToKeep = toKeep > samplesPerGeneration ? samplesPerGeneration : toKeep;
	fitnessFunc = fitFunc;
}

template<typename T>
inline GeneticAlgorithm<T>::~GeneticAlgorithm()
{
	delete(toSolveFor);
}

template<typename T>
inline void GeneticAlgorithm<T>::get_gen_parent(chromosome<T>& c, vector<chromosome<T>>* lastGenParents, int offset)
{
	get_parent<T>(c, *toSolveFor, lastGenParents, sequenceValues,
		parentsToKeep, offset,
		randomParentRangeChance, randParentChance,
		randomWheelSpinRangeChance, randwheelSpinChance,
		resetRangeChance, resetChance,
		fitnessFunc);
}

template<typename T>
inline void GeneticAlgorithm<T>::mutate_chromosome(chromosome<T>& c, int mutationChanceRange, int mutationChance,
	int numbits, int maxRange, int minRange,
	int mutation, bool forceMutation)
{
	int chance = get_random_int(mutationChanceRange, 1);
	if (chance <= mutationChance || forceMutation)
	{
		int m = mutation != -1 ? mutation : get_random_int(4);
		switch (m)
		{
			case 0:	//Shuffle the childs data
			{
				sample_shuffle<T>(c.sequence, get_random_int(c.sequence.size()));
				break;
			}
			case 1:	//Scramble the childs data
			{
				scramble<T>(c.sequence);
				break;
			}
			case 2:	//invert the childs bit data
			{
				bit_inversion<T>(c.sequence, numbits, maxRange, minRange);
				break;
			}
			case 3:	//Selection Sampling
			{
				c.sequence = selection_sample<T>(c.sequence, c.sequence.size());
				break;
			}
		}
	}
	c.matches = fitnessFunc(c.sequence, *toSolveFor);
}

template<typename T>
inline void GeneticAlgorithm<T>::pick_cross_over(vector<chromosome<T>>* lastGenParents, vector<chromosome<T>>* newGen, int offset, int numChildrenToPush, int crossOverChosen)
{
	chromosome<T> childA, childB;
	get_gen_parent(childA, lastGenParents, offset);
	get_gen_parent(childB, lastGenParents, offset);
	switch (crossOverChosen)
	{
		case 0:	//crossover data at single point for two children, use only one child if not enough space to keep other
		{
			multi_point_crossover<T>(childA.sequence, childB.sequence, get_random_int(childA.sequence.size()));
			break;
		}
		case 1: //crossover data at multiple points for two children, use only one child if not enough space to keep other
		{
			int end = get_random_int(childA.sequence.size());
			if (end == 0) end++;
			int start = get_random_int(end);

			multi_point_crossover<T>(childA.sequence, childB.sequence, end, start);
			break;
		}
		case 2: //Uniform_crossover
		{
			uniform_crossover<T>(childA.sequence, childB.sequence);
			break;
		}
		
	}
	mutate_chromosome(childA, mutationRangeChance, mutationChance,
		numBits, maxBitVal, minBitVal,
		-1, true);
	mutate_chromosome(childB, mutationRangeChance, mutationChance,
		numBits, maxBitVal, minBitVal,
		-1, true);
	
	if (numChildrenToPush > 1)
	{
		mtx.lock();
		newGen->push_back(childA);
		newGen->push_back(childB);
	}
	else
	{
		int which = get_random_int(2);
		mtx.lock();
		newGen->push_back(which ? childA : childB);
	}
	mtx.unlock();
}

template<typename T>
inline vector<chromosome<T>>* GeneticAlgorithm<T>::create_generation(vector<chromosome<T>>* lastGenParents, bool retainParents, bool higherFitness)
{
	vector<chromosome<T>>* newGen;
	//vector<thread> workers;
	if (lastGenParents != nullptr)
	{
		newGen = new vector<chromosome<T>>();
		// Fill in the rest of the children with crossover values, mutations, or random resets
		int numChildrenToAdd = samplesPerGeneration - parentsToKeep;
		int offsetToGetParent = higherFitness ? numChildrenToAdd : 0;
		int numParentsIndexTo = offsetToGetParent + parentsToKeep;
		for (int i = 0; i <= numChildrenToAdd; )
		{
			int crossOverChosen = get_random_int(3);
			int childrenToAdd = (crossOverChosen == 0 || crossOverChosen == 1) ? 2 : 1;
			i += childrenToAdd;
			if (i + childrenToAdd > numChildrenToAdd)
				childrenToAdd--;
			pick_cross_over(lastGenParents, newGen, offsetToGetParent, childrenToAdd, crossOverChosen);
			//workers.push_back(thread(&GeneticAlgorithm<T>::pick_cross_over, this, lastGenParents, newGen, offsetToGetParent, childrenToAdd, crossOverChosen));
		}
		//for (int i = 0; i < workers.size(); i++)
		//	workers.at(i).join();

		for (int i = offsetToGetParent; i < numParentsIndexTo; i++)
			newGen->push_back(lastGenParents->at(i));
	}
	else
	{
		newGen = create_population<T>(sequenceValues, *toSolveFor, samplesPerGeneration, fitnessFunc);
		bestMatchCount = higherFitness ? (newGen->end() - 1)->matches : newGen->begin()->matches;
	}
	quickSort<chromosome<T>>(*newGen, 0, newGen->size() - 1);
	if (lastGenParents != nullptr)
		delete(lastGenParents);

	return newGen;
}

template<typename T>
inline void GeneticAlgorithm<T>::solver(bool retainParents, bool higherFitness)
{
	vector<chromosome<T>>* lastGenParents = nullptr;
	bool foundSolution = false;
	start = chrono::steady_clock::now();
	while (!foundSolution)
	{
		vector<chromosome<T>>* newGen = create_generation(lastGenParents, retainParents, higherFitness);
		generation++;

		debug_check_print_best_chromosome(*newGen, higherFitness);
		chromosome<T>& best = higherFitness ? *(newGen->end() - 1) : *newGen->begin();
		if ((higherFitness && best.matches == toSolveFor->size()) || (!higherFitness && best.matches == 0) || (SPBGU > 0 && (generation - lastBestGen) >= SPBGU))
		{
			end = chrono::steady_clock::now();
			printBest(&best);
			foundSolution = true;
			break;
		}
		lastGenParents = newGen;
	}
}

template<typename T>
inline void GeneticAlgorithm<T>::printBest(chromosome<T>* best)
{
	cout << "Reached Matching Sequence on Generation: " << generation << " In: " << chrono::duration_cast<chrono::minutes>(end - start).count() << "min. !!!\nSolution is: ";
	int typeHash = typeid(T).hash_code();
	if(typeHash == typeid(int).hash_code() || typeHash == typeid(float).hash_code() || typeHash == typeid(double).hash_code())
		cout << convert_to_string<T>(best->sequence);
	else
		cout << convert_to_string_using_stringstream<T>(best->sequence);
}

template<typename T>
inline void GeneticAlgorithm<T>::debug_check_print_best_chromosome(vector<chromosome<T>>& data, bool higherFitness, bool printBest)
{
	chromosome<T>& best = higherFitness ? *(data.end() - 1) : *data.begin();
	if ((higherFitness && best.matches > bestMatchCount) || (!higherFitness && best.matches < bestMatchCount))
	{
		lastBestGen = generation;
		bestMatchCount = best.matches;
		if (printBest)
		{
			cout << "Generation: " << generation << endl;
			print_chromosome_data(best);
			//print_dif_amount<T>(best.sequence, *toSolveFor);
			cout << "\n" << endl;
		}
	}
}
