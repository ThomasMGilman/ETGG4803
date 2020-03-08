#pragma once
#include "Utilities.h"
#include <thread>
#include <mutex>
#include <chrono>

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

	int (*fitnessFunc)(vector<T>&, vector<T>&);		//Fitness Function pointer

	chrono::steady_clock::time_point start, end;

public:
	GeneticAlgorithm(const int& size, const int& samples, const int& parentsToKeep, vector<T> sequenceRange, int (*fitFunc)(vector<T>&, vector<T>&) = get_match_count<T>);

	~GeneticAlgorithm();

	void get_gen_parent(chromosome<T>& c, vector<chromosome<T>>* lastGenParents, int offset);

	void pick_cross_over(vector<chromosome<T>>* lastGenParents, vector<chromosome<T>>* newGen, int offset, int numChildrenToPush, int crossOverChosen);

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

	void solver(bool retainParents = true, bool higherFitness = true);

	void debug_check_print_best_chromosome(vector<chromosome<T>>& data, bool higherFitness);
};

template<typename T>
inline GeneticAlgorithm<T>::GeneticAlgorithm(const int& size, const int& samples, const int& toKeep, vector<T> sequenceRange, int (*fitFunc)(vector<T>&, vector<T>&))
{
	toSolveFor = new vector<T>();
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
inline void GeneticAlgorithm<T>::pick_cross_over(vector<chromosome<T>>* lastGenParents, vector<chromosome<T>>* newGen, int offset, int numChildrenToPush, int crossOverChosen)
{
	chromosome<T> childA, childB;
	get_gen_parent(childA, lastGenParents, offset);
	bool childBSet = false;

	switch (crossOverChosen)
	{
		case 0:	//crossover data at single point for two children, use only one child if not enough space to keep other
		{
			get_gen_parent(childB, lastGenParents, offset);
			childBSet = true;

			multi_point_crossover<T>(childA.sequence, childB.sequence, get_random_int(childA.sequence.size()));
			break;
		}
		case 1: //crossover data at multiple points for two children, use only one child if not enough space to keep other
		{
			get_gen_parent(childB, lastGenParents, offset);
			childBSet = true;

			int end = get_random_int(childA.sequence.size());
			if (end == 0) end++;
			int start = get_random_int(end);

			multi_point_crossover<T>(childA.sequence, childB.sequence, end, start);
			break;
		}
		case 2: //Uniform_crossover
		{
			get_gen_parent(childB, lastGenParents, offset);
			childA.sequence = uniform_crossover<T>(childA.sequence, childB.sequence);
			break;
		}
		case 3:	//Selection Sampling
		{
			get_gen_parent(childB, lastGenParents, offset);
			childA.sequence = selection_sample<T>(childB.sequence, childB.sequence.size());
			break;
		}
		case 4: //Force Mutation
		{
			mutate_chromosome<T>(childA, mutationRangeChance, mutationChance,
				numBits, maxBitVal, minBitVal,
				-1, true);
			break;
		}
	}
	mutate_chromosome<T>(childA, mutationRangeChance, mutationChance,
		numBits, maxBitVal, minBitVal,
		-1, true);
	childA.matches = fitnessFunc(childA.sequence, *toSolveFor);				// Use function pointer to call assigned fitnessfunc

	if (childBSet)
	{
		mutate_chromosome<T>(childB, mutationRangeChance, mutationChance,
			numBits, maxBitVal, minBitVal,
			-1, true);
		childB.matches = fitnessFunc(childB.sequence, *toSolveFor);			// Use function pointer to call assigned fitnessfunc

		mtx.lock();
		if (numChildrenToPush == 1)
			newGen->push_back(get_random_int(2) ? childB : childA);
		else
		{
			newGen->push_back(childA);
			newGen->push_back(childB);
		}
	}
	else
	{
		mtx.lock();
		newGen->push_back(childA);
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
			int crossOverChosen = get_random_int(5);
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
	{
		//cout << "newgeneration == lastgeneration : " << compare_generations_equal(*newGen, *lastGenParents) << endl;
		//cout << endl;
		delete(lastGenParents);
	}
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
		//print_chromosome_vec<T>(*newGen, generation);
		debug_check_print_best_chromosome(*newGen, higherFitness);

		if (higherFitness && (newGen->end() - 1)->matches == toSolveFor->size() || newGen->begin()->matches == 0)
		{
			end = chrono::steady_clock::now();
			chromosome<T> solution = higherFitness ? *(newGen->end() - 1) : *newGen->begin();
			cout << "Reached Matching Sequence on Generation: " << generation << " In: "<< chrono::duration_cast<chrono::minutes>(end-start).count() <<"min. !!!\nSolution is: ";
			int tHash = typeid(T).hash_code();
			if (tHash == typeid(float).hash_code() || tHash == typeid(int).hash_code() || tHash == typeid(double).hash_code())
				cout << convert_to_string<T>(solution.sequence) << endl;
			else
				cout << convert_to_string_using_stringstream<T>(solution.sequence) << endl;
			foundSolution = true;
			break;
		}
		lastGenParents = newGen;
	}
}

template<typename T>
inline void GeneticAlgorithm<T>::debug_check_print_best_chromosome(vector<chromosome<T>>& data, bool higherFitness)
{
	if ((higherFitness && (data.end() - 1)->matches > bestMatchCount))
	{
		bestMatchCount = (data.end() - 1)->matches;
		cout << "Generation: " << generation << endl;
		print_chromosome_data(*(data.end() - 1));
		print_dif_amount<T>((*(data.end() - 1)).sequence, *toSolveFor);
		cout << "\n" << endl;
	}
	else if (!higherFitness && data.begin()->matches < bestMatchCount)
	{
		bestMatchCount = data.begin()->matches;
		cout << "Generation: " << generation << endl;
		print_chromosome_data(*data.begin());
		print_dif_amount<T>((*data.begin()).sequence, *toSolveFor);
		cout << "\n" << endl;
	}
}
