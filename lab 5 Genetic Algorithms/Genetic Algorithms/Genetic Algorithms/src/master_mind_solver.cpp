#include "master_mind_solver.h"

MasterMind::MasterMind(const int& size, const int& samples, const int& toKeep)
{
	toSolveFor = new vector<int>();
	sizeOfProblem = size;
	samplesPerGeneration = samples;
	parentsToKeep = toKeep > samplesPerGeneration ? samplesPerGeneration : toKeep;


	*toSolveFor = create_chromosome_data();

	solver();
}

MasterMind::~MasterMind()
{
	delete(toSolveFor);
}

vector<int> MasterMind::create_chromosome_data()
{
	vector<int> sequence;
	for (int i = 0; i < sizeOfProblem; i++)
		sequence.push_back(rand() % 9 + 1);
	return sequence;
}

MasterMind::chromosome MasterMind::create_chromosome()
{
	chromosome c;
	c.sequence = create_chromosome_data();
	c.matches = utility.get_match_count(c.sequence, *toSolveFor);
	return c;
}

vector<MasterMind::chromosome>* MasterMind::populate()
{
	vector<chromosome>* generation = new vector<chromosome>();
	for (int i = 0; i < samplesPerGeneration; i++)
		generation->push_back(create_chromosome());
	return generation;
}

void MasterMind::mutate_chromosome(MasterMind::chromosome& c, int mutation, bool forceMutation)
{
	int chance = rand() % mutationRangeChance + 1;
	if (chance <= mutationChance || forceMutation)
	{
		int m = mutation != -1 ? mutation : rand() % 3;
		switch (m)
		{
			case 0:	//Shuffle the childs data
			{
				utility.sample_shuffle(c.sequence, rand() % c.sequence.size());
				break;
			}
			case 1:	//Scramble the childs data
			{
				utility.scramble(c.sequence);
				break;
			}
			case 2:	//invert the childs bit data
			{
				utility.bit_inversion(c.sequence, 4, 9, 1);
				break;
			}
		}
	}
}

bool MasterMind::random_reset_check(MasterMind::chromosome& c)
{
	int chance = rand() % resetRangeChance + 1;
	if (chance <= resetChance)
	{
		c = create_chromosome();
		return true;
	}
	return false;
}

void MasterMind::get_parent(chromosome& child, vector<MasterMind::chromosome>* lastGenParents, int offsetIntoParents)
{
	int index, chance = rand() % randomParentRangeChance + 1;
	if (chance <= randParentChance)
	{
		index = rand() % lastGenParents->size();
		child = lastGenParents->at(index);
	}
	else if (!random_reset_check(child))
	{
		chance = rand() % randomWheelSpinRangeChance + 1;
		if (chance <= randwheelSpinChance)
		{
			vector<chromosome> copy = *lastGenParents;
			child = roulette_wheel_selection(copy);
		}
		else
		{
			index = rand() % parentsToKeep + offsetIntoParents;
			child = lastGenParents->at(index);
		}
	}
}

vector<MasterMind::chromosome>* MasterMind::create_generation(vector<MasterMind::chromosome>* lastGenParents, bool retainParents)
{
	vector<chromosome>* newGen;
	if (lastGenParents != nullptr)
	{
		newGen = new vector<chromosome>();
		// Fill in the rest of the children with crossover values, mutations, or random resets
		int remaining = samplesPerGeneration - parentsToKeep;
		for (int i = 0; i < remaining;)
		{
			chromosome childA, childB;
			get_parent(childA, lastGenParents, remaining);

			bool childBSet = false;
			switch (rand() % 5)
			{
				case 0:	//crossover data at single point for two children, use only one child if not enough space to keep other
				{
					get_parent(childB, lastGenParents, remaining);
					childBSet = true;

					utility.multi_point_crossover(childA.sequence, childB.sequence, rand() % childA.sequence.size());
					break;
				}
				case 1: //crossover data at multiple points for two children, use only one child if not enough space to keep other
				{
					get_parent(childB, lastGenParents, remaining);
					childBSet = true;

					int end = rand() % childA.sequence.size();
					if (end == 0) end++;
					int start = rand() % end;
					utility.multi_point_crossover(childA.sequence, childB.sequence, end, start);
					break;
				}
				case 2: //Uniform_crossover
				{
					get_parent(childB, lastGenParents, remaining);
					childA.sequence = utility.uniform_crossover(childA.sequence, childB.sequence);
					break;
				}	
				case 3:	//Selection Sampling
				{
					get_parent(childB, lastGenParents, remaining);
					childA.sequence = utility.selection_sample(childB.sequence, childB.sequence.size());
					break;
				}
				case 4: //Force Mutation
				{
					mutate_chromosome(childA, -1, true);
					break;
				}
			}
			mutate_chromosome(childA);
			childA.matches = utility.get_match_count(childA.sequence, *toSolveFor);
			if (childBSet)
			{
				mutate_chromosome(childB);
				childB.matches = utility.get_match_count(childA.sequence, *toSolveFor);
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
		for (int i = remaining; i < lastGenParents->size(); i++)
			newGen->push_back(lastGenParents->at(i));
			
		delete(lastGenParents);
	}
	else
		newGen = populate();
	return newGen;
}

void MasterMind::solver(bool retainParents)
{
	vector<chromosome>* lastGenParents = nullptr;
	bool foundSolution = false;
	while (!foundSolution)
	{
		vector<chromosome>* newGen = create_generation(lastGenParents, retainParents);
		utility.quickSort<chromosome>(*newGen, 0, newGen->size() - 1);
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

void MasterMind::print_generation(vector<MasterMind::chromosome>* children)
{
	cout << "Generation: " << generation << endl;
	cout << "toSolveFor:\t\t" << utility.convert_to_string(*toSolveFor) << endl;
	for (chromosome c : *children)
	{
		cout << "matches: " << c.matches << " child:\t";
		cout << utility.convert_to_string(c.sequence) << endl;
	}
	cout << endl;
}

bool MasterMind::chromosome::operator>(chromosome& other)	const { return matches > other.matches; }
bool MasterMind::chromosome::operator<(chromosome& other)	const { return matches < other.matches; }
bool MasterMind::chromosome::operator==(chromosome& other)	const { return matches == other.matches; }
float MasterMind::chromosome::operator/(chromosome& other)	const { return (float)matches / (float)other.matches; }
float MasterMind::chromosome::operator/(float other)	const { return (float)matches / other; }