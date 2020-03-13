#pragma once
#include <stdafx.h>
#include "Utilities.h"

template<typename T>
class GeneticAlgorithm
{
private:
	bool debugPrintMode, debugPrintBest, retainParentHistory;
protected:
	vector<T>* toSolveFor = nullptr;
	vector<T> sequenceValues;
	int samplesPerGeneration;
	int parentsToKeep;
	int sizeOfProblem;
	int SPBGU;					//stagnation Period Befor Give Up
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

	bool allowDuplicateParent = false;

	// Timer
	chrono::steady_clock::time_point start, end;

public:
	GeneticAlgorithm(const int& size, const int& samples, const int& parentsToKeep, vector<T> sequenceRange, bool debugPrint, bool debugPrintBest, const int& stagnationPeriodBeforGiveUp);

	~GeneticAlgorithm();

	/////////////////// PARENT CREATION AND GETTER FUNCTIONS //////////////////////////////////////////////////////////////////////

	virtual vector<T> create_chromosome_data();

	chromosome<T> create_chromosome();

	vector<chromosome<T>>* create_population();

	bool random_reset_check(chromosome<T>& c);

	void get_gen_parent(chromosome<T>& c, vector<chromosome<T>>* lastGenParents, int offset);

	void get_chromosomes_to_manipulate(chromosome<T>& childA, chromosome<T>& childB, vector<chromosome<T>>* lastGenParents, int offset);

	/////////////////// FITNESS FUNCTION //////////////////////////////////////////////////////////////////////////////////////////

	virtual void fitness_function_check(chromosome<T>& c) = NULL;

	/////////////////// MUTATION FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////

	virtual void mutate_chromosome(chromosome<T>& c, int mutation = -1, bool forceMutation = false);

	/////////////////// CROSSOVER FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////

	/*sawp the contents of a and b up to the point of crossover
	should pass the children/copies of the parent vectors to this function to retain parent info*/
	virtual void multi_point_crossover(vector<T>& a, vector<T>& b, int end, int start = NULL);

	virtual void uniform_crossover(vector<T>& a, vector<T>& b);

	virtual void pick_cross_over(vector<chromosome<T>>* lastGenParents, vector<chromosome<T>>* newGen, int offset, int numChildrenToPush, int crossOverChosen, bool noRepeats);

	/////////////////// GENERATION PICKER AND SOLVER /////////////////////////////////////////////////////////////////////////////

	/*
	Create a new generation using last gen parents. If specified, will retain a requested number of parents to keep in the next generation until a better parent is found.
	Creates children through crossover functions, random or forced mutation, or even a random reset if resets are allowed.
	Upon creation of a new child, it is matched with the problem to solve for, to provide a fitness value to the child.

	ARGUMENTS
	lastGenParents	: Vector of last generations parents to use for the crossover functions
	retainParents	: Keep a fixed number of best parents in the next generation until a better one is found
	higherFitness	: Specifies whether to use a higher or lower fitness score for best parent.
	*/
	vector<chromosome<T>>* create_generation(vector<chromosome<T>>* lastGenParents, bool retainParents, bool higherFitness, bool noRepeats = false);

	void solver(bool retainParents = true, bool higherFitness = true);

	/////////////////// DEBUG FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////

	bool compare_generations_equal(vector<chromosome<T>>& a, vector<chromosome<T>>& b);

	virtual void print_chromosome_data(chromosome<T>& data);

	void print_chromosome_vec(vector<chromosome<T>>& data);

	void printBest(chromosome<T>* best);

	void debug_check_print_best_chromosome(vector<chromosome<T>>& data, bool higherFitness);
};

template<typename T>
inline GeneticAlgorithm<T>::GeneticAlgorithm(const int& size, const int& samples, const int& parentsToKeep, vector<T> sequenceRange, bool debugPrint, bool debugPrintBest, const int& stagnationPeriodBeforGiveUp)
{
	this->toSolveFor = new vector<T>();
	this->SPBGU = stagnationPeriodBeforGiveUp;
	this->sequenceValues = sequenceRange;
	this->sizeOfProblem = size;
	this->samplesPerGeneration = samples;
	this->parentsToKeep = parentsToKeep > samplesPerGeneration ? samplesPerGeneration : parentsToKeep;
	this->debugPrintMode = debugPrint;
	this->debugPrintBest = debugPrintBest;
}

template<typename T>
inline GeneticAlgorithm<T>::~GeneticAlgorithm()
{
	delete(toSolveFor);
}

/////////////////// PARENT CREATION AND GETTER FUNCTIONS //////////////////////////////////////////////////////////////////////

template<typename T>
inline vector<T> GeneticAlgorithm<T>::create_chromosome_data()
{
	vector<T> sequence;
	for (int i = 0; i < sizeOfProblem; i++)
		sequence.push_back(safe_copy_data_from_vector<T>(sequenceValues, get_random_int(sequenceValues.size())));
	return sequence;
}

template<typename T>
inline chromosome<T> GeneticAlgorithm<T>::create_chromosome()
{
	chromosome<T> c;
	c.sequence = create_chromosome_data();
	return c;
}

template<typename T>
inline vector<chromosome<T>>* GeneticAlgorithm<T>::create_population()
{
	vector<chromosome<T>>* population = new vector<chromosome<T>>();
	for (int i = 0; i < samplesPerGeneration; i++)
		population->push_back(create_chromosome());
	return population;
}

template<typename T>
inline bool GeneticAlgorithm<T>::random_reset_check(chromosome<T>& c)
{
	int chance = get_random_int(resetRangeChance, 1);
	if (chance <= resetChance)
	{
		c = create_chromosome();
		return true;
	}
	return false;
}

template<typename T>
inline void GeneticAlgorithm<T>::get_gen_parent(chromosome<T>& c, vector<chromosome<T>>* lastGenParents, int offset)
{
	int index, chance = get_random_int(randomParentRangeChance + 1);
	if (chance <= randParentChance)
	{
		index = get_random_int(lastGenParents->size());
		safe_copy_data_from_vector<chromosome<T>>(c, *lastGenParents, index);
	}
	else if (!random_reset_check(c))
	{
		chance = get_random_int(randomWheelSpinRangeChance, 1);
		if (chance <= randwheelSpinChance)
		{
			vector<chromosome<T>> copy;
			safe_copy_vector<chromosome<T>>(copy, *lastGenParents);
			c = roulette_wheel_selection<chromosome<T>>(copy, sizeOfProblem);
		}
		else
		{
			index = get_random_int(parentsToKeep, offset);
			safe_copy_data_from_vector<chromosome<T>>(c, *lastGenParents, index);
		}
	}
}

template<typename T>
inline void GeneticAlgorithm<T>::get_chromosomes_to_manipulate(chromosome<T>& childA, chromosome<T>& childB, vector<chromosome<T>>* lastGenParents, int offset)
{
	if (!allowDuplicateParent)
	{
		while (childA == childB)
		{
			get_gen_parent(childA, lastGenParents, offset);
			get_gen_parent(childB, lastGenParents, offset);
		}
	}
	else
	{
		get_gen_parent(childA, lastGenParents, offset);
		get_gen_parent(childB, lastGenParents, offset);
	}
	
	if (debugPrintMode)
	{
		cout << endl;
		cout << "ChildA: "; print_chromosome_data(childA);
		cout << "ChildB: "; print_chromosome_data(childB);
	}
}

/////////////////// MUTATION FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline void GeneticAlgorithm<T>::mutate_chromosome(chromosome<T>& c, int mutation, bool forceMutation)
{
	int chance = get_random_int(mutationRangeChance, 1);
	if (chance <= mutationChance || forceMutation)
	{
		int m = mutation != -1 ? mutation : get_random_int(3);
		c.mutationChosen = m;
		if(debugPrintMode) 
			cout << "Mutation: " << to_string(m) << endl;
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
				bit_inversion<T>(c.sequence, numBits, maxBitVal, minBitVal);
				break;
			}
			case 3:	//Selection Sampling
			{
				c.sequence = selection_sample<T>(c.sequence, c.sequence.size());
				break;
			}
		}
	}
	fitness_function_check(c);
}

/////////////////// CROSSOVER FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline void GeneticAlgorithm<T>::multi_point_crossover(vector<T>& a, vector<T>& b, int end, int start)
{
	if (a.size() != b.size())
		throw new exception("passed mismatched vector sizes!!!");
	else if (end >= a.size() || end < 0 || (start != NULL && (start >= a.size() || start < 0 || start > end)))
		throw new exception("Given an crossover point greater than or less than size of vectors");

	int begining = start != NULL ? start : 0;
	for (int i = begining; i <= end; i++)
		swap_data(a.at(i), b.at(i));
}

template<typename T>
inline void GeneticAlgorithm<T>::uniform_crossover(vector<T>& a, vector<T>& b)
{
	vector<T> newA, newB;
	if (a.size() != b.size())
		throw new exception("passed mismatched vector sizes!!!");

	for (int i = 0; i < a.size(); i++)
	{
		int which = get_random_int(2);
		newA.push_back(which ? a.at(i) : b.at(i));
		newB.push_back(which ? b.at(i) : a.at(i));
	}
	a = newA;
	b = newB;
}

template<typename T>
inline void GeneticAlgorithm<T>::pick_cross_over(vector<chromosome<T>>* lastGenParents, vector<chromosome<T>>* newGen, int offset, int numChildrenToPush, int crossOverChosen, bool noRepeats)
{
	chromosome<T> childA, childB;
	get_chromosomes_to_manipulate(childA, childB, lastGenParents, offset);
	if(debugPrintMode)
		cout << "CrossOver: " << to_string(crossOverChosen) << endl;
	switch (crossOverChosen)
	{
		case 0:	//crossover data at single point for two children, use only one child if not enough space to keep other
		{
			multi_point_crossover(childA.sequence, childB.sequence, get_random_int(childA.sequence.size()));
			break;
		}
		case 1: //crossover data at multiple points for two children, use only one child if not enough space to keep other
		{
			int end = get_random_int(childA.sequence.size());
			if (end == 0) end++;
			int start = get_random_int(end);

			multi_point_crossover(childA.sequence, childB.sequence, end, start);
			break;
		}
		case 2: //Uniform_crossover
		{
			uniform_crossover(childA.sequence, childB.sequence);
			break;
		}
	}
	childA.crossOverChosen = crossOverChosen;
	childB.crossOverChosen = crossOverChosen;
	mutate_chromosome(childA);
	mutate_chromosome(childB);
	
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

/////////////////// GENERATION PICKER AND SOLVER /////////////////////////////////////////////////////////////////////////////

template<typename T>
inline vector<chromosome<T>>* GeneticAlgorithm<T>::create_generation(vector<chromosome<T>>* lastGenParents, bool retainParents, bool higherFitness, bool noRepeats)
{
	vector<chromosome<T>>* newGen;
	//vector<thread> workers;
	if (lastGenParents != nullptr)
	{
		newGen = new vector<chromosome<T>>();
		// Fill in the rest of the children with crossover values, mutations, or random resets
		int numChildrenToAdd = samplesPerGeneration - parentsToKeep - 1;
		int offsetToGetParent = higherFitness ? numChildrenToAdd : 0;
		int numParentsIndexTo = offsetToGetParent + parentsToKeep;
		for (int i = 0; i <= numChildrenToAdd; )
		{
			int crossOverChosen = get_random_int(3);
			int childrenToAdd = (crossOverChosen == 0 || crossOverChosen == 1) ? 2 : 1;
			i += childrenToAdd;
			if (i + childrenToAdd > numChildrenToAdd)
				childrenToAdd--;
			pick_cross_over(lastGenParents, newGen, offsetToGetParent, childrenToAdd, crossOverChosen, noRepeats);
			//workers.push_back(thread(&GeneticAlgorithm<T>::pick_cross_over, this, lastGenParents, newGen, offsetToGetParent, childrenToAdd, crossOverChosen));
		}
		//for (int i = 0; i < workers.size(); i++)
		//	workers.at(i).join();

		for (int i = offsetToGetParent; i < numParentsIndexTo; i++)
			newGen->push_back(lastGenParents->at(i));
	}
	else
	{
		newGen = create_population();
		for(int i = 0; i < newGen->size(); i++)
			fitness_function_check(newGen->at(i));
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
		if (debugPrintMode)
			print_chromosome_vec(*newGen);

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

/////////////////// DEBUG FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline bool GeneticAlgorithm<T>::compare_generations_equal(vector<chromosome<T>>& a, vector<chromosome<T>>& b)
{
	for (int i = 0; i < a.size(); i++)
	{
		cout << "index: " << to_string(i) << endl;
		print_chromosome_data(a[i]);
		print_chromosome_data(b[i]);
		cout << endl;
		if (get_match_difference_offset(a[i].sequence, b[i].sequence))
		{
			cout << "difference at: " << to_string(i) << endl;
			print_chromosome_data(a[i]);
			print_chromosome_data(b[i]);
			return false;
		}
	}
	return true;
}

template<typename T>
inline void GeneticAlgorithm<T>::print_chromosome_data(chromosome<T>& data)
{
	cout << "\tData:\t\t";
	const char* typeHash = typeid(T).name();
	if (typeHash == typeid(int).name() || typeHash == typeid(float).name() || typeHash == typeid(double).name())
		cout << convert_to_string<T>(data.sequence) << endl;
	else
		cout << convert_to_string_using_stringstream<T>(data.sequence) << endl;
	cout << "\tmatches:\t" << (data.matches == -1 ? "N/A" : to_string(data.matches)) << endl;
	cout << "\tCrossOver:\t" << (data.crossOverChosen == -1 ? "N/A" : to_string(data.crossOverChosen)) << endl;
	cout << "\tMutation:\t" << (data.mutationChosen == -1 ? "N/A" : to_string(data.mutationChosen)) << endl;
}

template<typename T>
inline void GeneticAlgorithm<T>::print_chromosome_vec(vector<chromosome<T>>& data)
{
	cout << "Generation: " << generation << endl;
	for (chromosome<T> c : data)
		print_chromosome_data(c);
	cout << "\n" << endl;
}

template<typename T>
inline void GeneticAlgorithm<T>::printBest(chromosome<T>* best)
{
	cout << "Reached Matching Sequence on Generation: " << generation << " In: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "mSec. " <<  "!!!\nSolution is: ";
	print_chromosome_data(*best);
}

template<typename T>
inline void GeneticAlgorithm<T>::debug_check_print_best_chromosome(vector<chromosome<T>>& data, bool higherFitness)
{
	chromosome<T>& best = higherFitness ? *(data.end() - 1) : *data.begin();
	if ((higherFitness && best.matches > bestMatchCount) || (!higherFitness && best.matches < bestMatchCount))
	{
		lastBestGen = generation;
		bestMatchCount = best.matches == -1 ? (higherFitness ? 0 : sizeOfProblem) : best.matches;
		if (debugPrintBest || debugPrintMode)
		{
			cout << "Generation: " << generation << endl;
			print_chromosome_data(best);
			//print_dif_amount<T>(best.sequence, *toSolveFor);
			cout << "\n" << endl;
		}
	}
}
