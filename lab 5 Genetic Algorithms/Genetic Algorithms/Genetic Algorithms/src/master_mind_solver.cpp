#include "master_mind_solver.h"

MasterMind::MasterMind(const int& size, const int& samples, const int& toKeep, vector<int>& sequenceRange)
{
	toSolveFor = new vector<int>();
	sequenceValues = sequenceRange;
	sizeOfProblem = size;
	samplesPerGeneration = samples;
	parentsToKeep = toKeep > samplesPerGeneration ? samplesPerGeneration : toKeep;


	*toSolveFor = utility.create_chromosome_data<int>(sequenceValues, size);

	solver();
}

MasterMind::~MasterMind()
{
	delete(toSolveFor);
}

void MasterMind::get_parent(Utilities::chromosome<int>& c, vector<Utilities::chromosome<int>>* lastGenParents, int offset)
{
	utility.get_parent<int>(c, *toSolveFor, lastGenParents, sequenceValues,
		parentsToKeep, offset,
		randomParentRangeChance, randParentChance,
		randomWheelSpinRangeChance, randwheelSpinChance,
		resetRangeChance, resetChance);
}

vector<Utilities::chromosome<int>>* MasterMind::create_generation(vector<Utilities::chromosome<int>>* lastGenParents, bool retainParents)
{
	vector<Utilities::chromosome<int>>* newGen;
	if (lastGenParents != nullptr)
	{
		newGen = new vector<Utilities::chromosome<int>>();
		// Fill in the rest of the children with crossover values, mutations, or random resets
		int offset = samplesPerGeneration - parentsToKeep;
		for (int i = 0; i < offset;)
		{
			Utilities::chromosome<int> childA, childB;
			get_parent(childA, lastGenParents, offset);

			bool childBSet = false;
			switch (rand() % 5)
			{
			case 0:	//crossover data at single point for two children, use only one child if not enough space to keep other
			{
				get_parent(childB, lastGenParents, offset);
				childBSet = true;

				utility.multi_point_crossover<int>(childA.sequence, childB.sequence, rand() % childA.sequence.size());
				break;
			}
			case 1: //crossover data at multiple points for two children, use only one child if not enough space to keep other
			{
				get_parent(childB, lastGenParents, offset);
				childBSet = true;

				int end = rand() % childA.sequence.size();
				if (end == 0) end++;
				int start = rand() % end;
				utility.multi_point_crossover<int>(childA.sequence, childB.sequence, end, start);
				break;
			}
			case 2: //Uniform_crossover
			{
				get_parent(childB, lastGenParents, offset);
				childA.sequence = utility.uniform_crossover<int>(childA.sequence, childB.sequence);
				break;
			}
			case 3:	//Selection Sampling
			{
				get_parent(childB, lastGenParents, offset);
				childA.sequence = utility.selection_sample<int>(childB.sequence, childB.sequence.size());
				break;
			}
			case 4: //Force Mutation
			{
				utility.mutate_chromosome<int>(childA, mutationRangeChance, mutationChance,
					4, 9, 1,
					-1, true);
				break;
			}
			}
			utility.mutate_chromosome<int>(childA, mutationRangeChance, mutationChance,
				4, 9, 1,
				-1, true);
			childA.matches = utility.get_match_count<int>(childA.sequence, *toSolveFor);
			if (childBSet)
			{
				utility.mutate_chromosome<int>(childB, mutationRangeChance, mutationChance,
					4, 9, 1,
					-1, true);
				childB.matches = utility.get_match_count<int>(childA.sequence, *toSolveFor);
				if (i == offset - 1)
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
		for (int i = offset; i < lastGenParents->size(); i++)
			newGen->push_back(lastGenParents->at(i));

		delete(lastGenParents);
	}
	else
		newGen = utility.create_population<int>(sequenceValues, *toSolveFor, samplesPerGeneration);
	return newGen;
}

void MasterMind::solver(bool retainParents)
{
	vector<Utilities::chromosome<int>>* lastGenParents = nullptr;
	bool foundSolution = false;
	while (!foundSolution)
	{
		vector<Utilities::chromosome<int>>* newGen = create_generation(lastGenParents, retainParents);
		utility.quickSort<Utilities::chromosome<int>>(*newGen, 0, newGen->size() - 1);
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
