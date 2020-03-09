#include "n_queens_solver.h"

void NQueensSolver::pick_cross_over(vector<chromosome<int>>* lastGenParents, vector<chromosome<int>>* newGen, int offset)
{
	chromosome<int> child;
	get_parent<int>(child, lastGenParents, sequenceValues,
		parentsToKeep, offset,
		randomParentRangeChance, randParentChance,
		randomWheelSpinRangeChance, randwheelSpinChance,
		resetRangeChance, resetChance,
		check_queens);
	mutate_chromosome(child, mutationRangeChance, mutationChance,
		numBits, maxBitVal, minBitVal,
		-1, true);
	child.matches = check_queens(child.sequence);
	lock_guard<mutex> lock(mtx);
	newGen->push_back(child);
}

vector<chromosome<int>>* NQueensSolver::create_generation(vector<chromosome<int>>* lastGenParents, bool retainParents)
{
	vector<chromosome<int>>* newGen;
	//vector<thread> workers;
	if (lastGenParents != nullptr)
	{
		newGen = new vector<chromosome<int>>();
		// Fill in the rest of the children with crossover values, mutations, or random resets
		int numChildrenToAdd = samplesPerGeneration - parentsToKeep;
		for (int i = 0; i <= numChildrenToAdd; i++)
		{
			pick_cross_over(lastGenParents, newGen, 0);
			//workers.push_back(thread(&NQueensSolver::pick_cross_over, this, lastGenParents, newGen, 0));
		}
		//for (int i = 0; i < workers.size(); i++)
		//	workers.at(i).join();

		for (int i = 0; i < parentsToKeep; i++)
			newGen->push_back(lastGenParents->at(i));
	}
	else
	{
		newGen = create_population<int>(sequenceValues, sizeOfProblem, samplesPerGeneration, check_queens);
		bestMatchCount = newGen->begin()->matches;
	}
	quickSort<chromosome<int>>(*newGen, 0, newGen->size() - 1);
	if (lastGenParents != nullptr)
		delete(lastGenParents);

	return newGen;
}

void NQueensSolver::solver(bool retainParents)
{
	vector<chromosome<int>>* lastGenParents = nullptr;
	bool foundSolution = false;
	start = chrono::steady_clock::now();
	while (!foundSolution)
	{
		vector<chromosome<int>>* newGen = create_generation(lastGenParents, retainParents);
		generation++;

		debug_check_print_best_chromosome(*newGen, false, false);
		chromosome<int>& best = *newGen->begin();
		if (best.matches == sizeOfProblem || (SPBGU > 0 && (generation - lastBestGen) >= SPBGU))
		{
			end = chrono::steady_clock::now();
			printBest(&best);
			int numQueens = 0;
			for (int i : best.sequence)
				if (i != 0) numQueens++;
			cout << " Number of Queens on board: " << numQueens << endl;
			foundSolution = true;
			break;
		}
		lastGenParents = newGen;
	}
}
