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
	return generation;
}

vector<MasterMind::chromosome>* MasterMind::create_generation(vector<MasterMind::chromosome>* lastGenParents, bool retainParents)
{
	vector<chromosome>* newGen;
	if (lastGenParents != nullptr)
	{
		newGen = new vector<chromosome>;
		for (int i = 0; i < lastGenParents->size(); i++)
			newGen->push_back(lastGenParents->at(i));

		// Fill in the rest of the children with crossover values, mutations, or random resets
		int remaining = samplesPerGeneration - newGen->size();
		for (int i = 0; i < remaining;)
		{
			chromosome childA = lastGenParents->at(rand() % lastGenParents->size());
			chromosome childB;
			bool childBSet = false;
			int choice = rand() % 8;
			switch (choice)
			{
				case 0:	//Shuffle the childs data
				{
					sample_shuffle<int>(childA.sequence, rand() % childA.sequence.size());
					break;
				}
				case 1:	//Scramble the childs data
				{
					scramble<int>(childA.sequence);
					break;
				}
				case 2:	//invert the childs bit data
				{
					bit_inversion(childA.sequence, 4, 9, 1);
					break;
				}
				case 3:	//crossover data at single point for two children, use only one child if not enough space to keep other
				{
					childB = lastGenParents->at(rand() % lastGenParents->size());
					childBSet = true;
					multi_point_crossover<int>(childA.sequence, childB.sequence, rand() % (childA.sequence.size() - 1));
					break;
				}
				case 4: //crossover data at multiple points for two children, use only one child if not enough space to keep other
				{
					childB = lastGenParents->at(rand() % lastGenParents->size());
					childBSet = true;
					int end = rand() % (childA.sequence.size() - 1);
					int start = rand() % end;
					multi_point_crossover<int>(childA.sequence, childB.sequence, end, start);
					break;
				}
				case 5: //Uniform_crossover
				{
					childB = lastGenParents->at(rand() % lastGenParents->size());
					childA.sequence = uniform_crossover<int>(childA.sequence, childB.sequence);
					break;
				}	
				case 6:	//Selection Sampling
				{
					childB = lastGenParents->at(rand() % lastGenParents->size());
					childA.sequence = selection_sample<int>(childB.sequence, rand() % (childB.sequence.size() - 1));
					break;
				}	
				case 7: //random reset child
				{
					childA.sequence = create_chromosome_data();
					break;
				}
			}
			childA.matches = get_match_count<int>(childA.sequence, *toSolveFor);
			if (childBSet)
			{
				childB.matches = get_match_count<int>(childA.sequence, *toSolveFor);
				if (i == remaining - 1)
					newGen->push_back(rand() % 2 ? childB : childA);
				else
				{
					newGen->push_back(childA);
					newGen->push_back(childB);
					i++;
				}
			}
			else
				newGen->push_back(childA);
			i++;
		}
		delete(lastGenParents);
	}
	else
		newGen = populate();
	return newGen;
}

void MasterMind::solver(vector<chromosome>* lastGenParents, bool retainParents)
{
	vector<chromosome>* newGen = create_generation(lastGenParents, retainParents);
	quickSort(newGen, 0, newGen->size() - 1);
	generation++;
	print_generation(newGen);
	if ((newGen->end() - 1)->matches == toSolveFor->size())
	{
		cout << "Reached Matching Sequence!!! Solution is: " << convert_to_String(&(newGen->end() - 1)->sequence, (newGen->end() - 1)->sequence.size()) << endl;
		cout << "To solve for was: " << convert_to_String(toSolveFor, toSolveFor->size()) << endl;
	}
	else
		solver(newGen, retainParents);
}

void MasterMind::print_generation(vector<MasterMind::chromosome>* children)
{
	cout << "Generation: " << generation << endl;
	for (chromosome c : *children)
		cout << "matches: "<< c.matches << " child: "<< convert_to_String(&c.sequence, c.sequence.size()) << endl;
	cout << endl;
}

bool MasterMind::chromosome::operator>(chromosome& other)	const { return matches > other.matches; }
bool MasterMind::chromosome::operator<(chromosome& other)	const { return matches < other.matches; }
bool MasterMind::chromosome::operator==(chromosome& other)	const { return matches == other.matches; }