#pragma once
#include "Utilities.h"

template<typename T>
class GeneticAlgorithm
{
private:
protected:
	Utilities utility;
	vector<T>* toSolveFor;
	vector<T> sequenceValues;
	int samplesPerGeneration;
	int parentsToKeep;
	int sizeOfProblem;
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
	GeneticAlgorithm(const int& size, const int& samples, const int& parentsToKeep, vector<T> sequenceRange);

	~GeneticAlgorithm();

	void get_parent(Utilities::chromosome<T>& c, vector<Utilities::chromosome<T>>* lastGenParents, int offset);

	virtual vector<Utilities::chromosome<T>>* create_generation(vector<Utilities::chromosome<T>>* lastGenParents, bool retainParents) = NULL;

	void solver(bool retainParents = true);
};

template<typename T>
inline GeneticAlgorithm<T>::GeneticAlgorithm(const int& size, const int& samples, const int& toKeep, vector<T> sequenceRange)
{
	toSolveFor = new vector<T>();
	sequenceValues = sequenceRange;
	sizeOfProblem = size;
	samplesPerGeneration = samples;
	parentsToKeep = toKeep > samplesPerGeneration ? samplesPerGeneration : toKeep;
}

template<typename T>
inline GeneticAlgorithm<T>::~GeneticAlgorithm()
{
	delete(toSolveFor);
}

template<typename T>
inline void GeneticAlgorithm<T>::get_parent(Utilities::chromosome<T>& c, vector<Utilities::chromosome<T>>* lastGenParents, int offset)
{
	utility.get_parent<T>(c, *toSolveFor, lastGenParents, sequenceValues,
		parentsToKeep, offset,
		randomParentRangeChance, randParentChance,
		randomWheelSpinRangeChance, randwheelSpinChance,
		resetRangeChance, resetChance);
}

template<typename T>
inline void GeneticAlgorithm<T>::solver(bool retainParents)
{
	vector<Utilities::chromosome<T>>* lastGenParents = nullptr;
	bool foundSolution = false;
	while (!foundSolution)
	{
		vector<Utilities::chromosome<T>>* newGen = create_generation(lastGenParents, retainParents);
		utility.quickSort<Utilities::chromosome<T>>(*newGen, 0, newGen->size() - 1);
		generation++;
		//print_generation(newGen);
		if ((newGen->end() - 1)->matches == toSolveFor->size())
		{
			cout << "Reached Matching Sequence on Generation: " << generation << " !!!\nSolution is: " << utility.convert_to_string((newGen->end() - 1)->sequence) << endl;
			foundSolution = true;
			break;
		}
		lastGenParents = newGen;
	}
}
