#include "MasterMind.h"
#include <iostream>

MasterMind::MasterMind(const int& size, const int& samples, const int& toKeep)
{
	toSolveFor = new vector<int>;
	samplesPerGeneration = samples;
	parentsToKeep = toKeep;

	for (int i = 0; i < size; i++)
		toSolveFor->push_back(rand() % 9);

	populate();
	solver();
}

MasterMind::~MasterMind()
{
	delete(toSolveFor);
}

void MasterMind::inversion(chromosome c)
{
	for (int i : c.sequence)
		i = ~i;
}

MasterMind::chromosome MasterMind::create_chromosome()
{
	chromosome c;
	for (int i = 0; i < toSolveFor->size(); i++)
		c.sequence.push_back(rand() % 9);
	return c;
}

vector<MasterMind::chromosome>* MasterMind::populate()
{
	vector<chromosome>* generation = new vector<chromosome>();
	for (int i = 0; i < samplesPerGeneration; i++)
		generation->push_back(create_chromosome());
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