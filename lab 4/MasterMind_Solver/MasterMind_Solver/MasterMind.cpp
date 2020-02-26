#include "MasterMind.h"
#include <iostream>

MasterMind::MasterMind(const int& size, const int& samples, const int& toKeep)
{
	toSolveFor = new vector<int>;
	samplesPerGeneration = samples;
	parentsToKeep = toKeep;

	toSolveFor = &create_chromosome_data();

	populate();
	solver();
}

MasterMind::~MasterMind()
{
	delete(toSolveFor);
}

vector<int> MasterMind::create_chromosome_data()
{
	vector<int> sequence;
	for (int i = 0; i < toSolveFor->size(); i++)
		sequence.push_back(rand() % 9 + 1);
	return sequence;
}

vector<MasterMind::chromosome>* MasterMind::populate()
{
	vector<chromosome>* generation = new vector<chromosome>();
	for (int i = 0; i < samplesPerGeneration; i++)
	{
		chromosome c;
		c.sequence = create_chromosome_data();
		generation->push_back(c);
	}
}

void MasterMind::solver(vector<chromosome>* lastGenParents)
{
	vector<chromosome>* gen; 
	int thisGenSamples = samplesPerGeneration, i = 0;
	if (lastGenParents != nullptr)
	{
		gen = new vector<chromosome>;
		i += lastGenParents->size();
		thisGenSamples -= lastGenParents->size();
		for (int i = 0; i < lastGenParents->size(); i++)
			gen->push_back(lastGenParents->at(i));
		delete(lastGenParents);

		// Fill in the rest of the children with crossover values, mutations, or random resets
		
	}
	else
		gen = populate();

	for (i; i < samplesPerGeneration; i++)
	{

	}
}

bool MasterMind::chromosome::operator>(chromosome& other)	const { return matches > other.matches; }
bool MasterMind::chromosome::operator<(chromosome& other)	const { return matches < other.matches; }
bool MasterMind::chromosome::operator==(chromosome& other)	const { return matches == other.matches; }