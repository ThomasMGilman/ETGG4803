#pragma once
#include "Utilities.h"
#include <thread>
#include <mutex>

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

	mutex* mtx;

public:
	GeneticAlgorithm(const int& size, const int& samples, const int& parentsToKeep, vector<T> sequenceRange, int (*fitFunc)(vector<T>&, vector<T>&) = get_match_count<T>);

	~GeneticAlgorithm();

	void get_gen_parent(chromosome<T>& c, vector<chromosome<T>>* lastGenParents, int offset);

	void pick_cross_over(vector<chromosome<T>>* lastGenParents, vector<chromosome<T>>* newGen, int offset);

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
	mtx = new mutex;
}

template<typename T>
inline GeneticAlgorithm<T>::~GeneticAlgorithm()
{
	delete(toSolveFor);
	delete(mtx);
}

template<typename T>
inline void GeneticAlgorithm<T>::get_gen_parent(chromosome<T>& c, vector<chromosome<T>>* lastGenParents, int offset)
{
	get_parent<T>(c, *toSolveFor, lastGenParents, sequenceValues,
		parentsToKeep, offset,
		randomParentRangeChance, randParentChance,
		randomWheelSpinRangeChance, randwheelSpinChance,
		resetRangeChance, resetChance,
		*mtx,
		fitnessFunc);
}

template<typename T>
inline void pick_cross_over(vector<chromosome<T>>* lastGenParents, vector<chromosome<T>>* newGen, int offset)
{
	chromosome<T> childA, childB;
	get_gen_parent(childA, lastGenParents, offset);
	bool childBSet = false;
	switch (get_random_int(*mtx, 5))
	{
		case 0:	//crossover data at single point for two children, use only one child if not enough space to keep other
		{
			get_gen_parent(childB, lastGenParents, offset);
			childBSet = true;

			multi_point_crossover<T>(childA.sequence, childB.sequence, get_random_int(*mtx, childA.sequence.size()));
			break;
		}
		case 1: //crossover data at multiple points for two children, use only one child if not enough space to keep other
		{
			get_gen_parent(childB, lastGenParents, offset);
			childBSet = true;

			int end = get_random_int(*mtx, childA.sequence.size());
			if (end == 0) end++;
			int start = rand() % end;
			multi_point_crossover<T>(childA.sequence, childB.sequence, end, start);
			break;
		}
		case 2: //Uniform_crossover
		{
			get_gen_parent(childB, lastGenParents, offset);
			childA.sequence = uniform_crossover<T>(*mtx, childA.sequence, childB.sequence);
			break;
		}
		case 3:	//Selection Sampling
		{
			get_gen_parent(childB, lastGenParents, offset);
			childA.sequence = selection_sample<T>(*mtx, childB.sequence, childB.sequence.size());
			break;
		}
		case 4: //Force Mutation
		{
			mutate_chromosome<T>(*mtx, childA, mutationRangeChance, mutationChance,
				numBits, maxBitVal, minBitVal,
				-1, true);
			break;
		}
	}
	mutate_chromosome<T>(*mtx, childA, mutationRangeChance, mutationChance,
		numBits, maxBitVal, minBitVal,
		-1, true);
	childA.matches = fitnessFunc(childA.sequence, *toSolveFor);
	if (childBSet)
	{
		mutate_chromosome<T>(*mtx, childB, mutationRangeChance, mutationChance,
			numBits, maxBitVal, minBitVal,
			-1, true);
		childB.matches = fitnessFunc(childA.sequence, *toSolveFor);
		mtx->lock();
		
		if (newGen->size() == samplesPerGeneration - offset)
			newGen->push_back(get_random_int(*mtx, 2) ? childB : childA);
		else
		{
			newGen->push_back(childA);
			newGen->push_back(childB);
		}
		mtx->unlock();
	}
	else
		newGen->push_back(childA);
}

template<typename T>
inline vector<chromosome<T>>* GeneticAlgorithm<T>::create_generation(vector<chromosome<T>>* lastGenParents, bool retainParents, bool higherFitness)
{
	vector<chromosome<T>>* newGen;
	vector<thread> workers;
	if (lastGenParents != nullptr)
	{
		newGen = new vector<chromosome<T>>();
		// Fill in the rest of the children with crossover values, mutations, or random resets
		int offset = higherFitness ? samplesPerGeneration - parentsToKeep : 0;
		int numParents = offset + parentsToKeep;
		for (int i = 0; i < samplesPerGeneration - parentsToKeep; i++)
		{
			pick_cross_over(lastGenParents, newGen, numParents);
		}
		for (int i = offset; i < numParents; i++)
			newGen->push_back(lastGenParents->at(i));

		delete(lastGenParents);
	}
	else
	{
		newGen = create_population<T>(sequenceValues, *toSolveFor, samplesPerGeneration, fitnessFunc);
		bestMatchCount = higherFitness ? (newGen->end() - 1)->matches : newGen->begin()->matches;
	}
	return newGen;
}

template<typename T>
inline void GeneticAlgorithm<T>::solver(bool retainParents, bool higherFitness)
{
	vector<chromosome<T>>* lastGenParents = nullptr;
	bool foundSolution = false;
	while (!foundSolution)
	{
		vector<chromosome<T>>* newGen = create_generation(lastGenParents, retainParents, higherFitness);
		quickSort<chromosome<T>>(*newGen, 0, newGen->size() - 1);
		generation++;
		//print_chromosome_vec<T>(*newGen, generation);
		debug_check_print_best_chromosome(*newGen, higherFitness);

		if (higherFitness && (newGen->end() - 1)->matches == toSolveFor->size())
		{
			cout << "Reached Matching Sequence on Generation: " << generation << " !!!\nSolution is: " << convert_to_string<T>((newGen->end() - 1)->sequence) << endl;
			foundSolution = true;
			break;
		}
		else if (newGen->begin()->matches == 0)
		{
			cout << "Reached Matching Sequence on Generation: " << generation << " !!!\nSolution is: " << convert_to_string<T>(newGen->begin()->sequence) << endl;
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
		cout << "\n" << endl;
	}
	else if (!higherFitness && data.begin()->matches < bestMatchCount)
	{
		bestMatchCount = data.begin()->matches;
		cout << "Generation: " << generation << endl;
		print_chromosome_data(*data.begin());
		cout << "\n" << endl;
	}
}
