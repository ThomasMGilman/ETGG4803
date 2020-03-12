#include <stdafx.h>
#include "n_queens_solver.h"

vector<int> NQueensSolver::create_chromosome_data()
{
	vector<int> availableValues, data;
	for (int space : sequenceValues)
		availableValues.push_back(space);
	for (int i = 0; i < sizeOfProblem; i++)
	{
		int indexChosen = get_random_int(availableValues.size());
		data.push_back(availableValues.at(indexChosen));
		availableValues.erase(availableValues.begin() + indexChosen);
	}
	return data;
}

void NQueensSolver::fitness_function_check(chromosome<int>& c)
{
	if(c.sequence.size() != sizeOfProblem)
		throw new exception("BAD Vector Size ERROR!! Vec A does not match size of problem!!!");
	c.matches = 0;
	for (int i = 0; i < sizeOfProblem; i++)
	{
		if (c.sequence[i] == 0)
			c.matches++;

		for (int j = 0; j < sizeOfProblem; j++)
		{
			if (j == i) continue;
			float dRow = abs(c.sequence[i] - c.sequence[j]);
			float dCol = abs(i - j);

			// Check same row or same column should never have a same row issue as long as crossovers work correctly
			if (c.sequence[i] == c.sequence[j] || abs(dRow / dCol) == 1)
				c.matches += sizeOfProblem;
		}
	}
}

void NQueensSolver::multi_point_crossover(vector<int>& a, vector<int>& b, int end, int start)
{
	if (a.size() != b.size())
		throw new exception("Mismatched Size ERROR!! Vec A does not match size of Vec B!!!");
	if (start == 0 && end == a.size() - 1)
	{
		vector<int> tmp = b;
		b = a;
		a = tmp;
	}
	else
	{
		vector<int> newA, newB;
		unordered_set<int> aContains, bContains;
		for (int i = 0; i < a.size(); i++)
		{
			if (i >= start && i <= end)
			{
				newA.push_back(-1);
				newB.push_back(-1);
			}
			else
			{
				newA.push_back(a[i]);
				aContains.insert(a[i]);
				newB.push_back(b[i]);
				bContains.insert(b[i]);
			}
		}

		for (int i = start; i <= end; i++)
		{
			bool aSet = false, bSet = false;
			for(int j = i; j <= a.size(); j++)
			{
				if (j == a.size()) j = 0;
				if (!aSet && aContains.find(b[j]) == aContains.end())
				{
					newA[i] = b[j];
					aContains.insert(b[j]);
					aSet = true;
				}
				if (!bSet && bContains.find(a[j]) == bContains.end())
				{
					newB[i] = a[j];
					bContains.insert(a[j]);
					bSet = true;
				}
				if (aSet && bSet) break;
			}
		}
		a = newA;
		b = newB;
	}
}

void NQueensSolver::uniform_crossover(vector<int>& a, vector<int>& b)
{
	if (a.size() != b.size())
		throw new exception("Mismatched Size ERROR!! Vec A does not match size of Vec B!!!");
	unordered_set<int> aContains, bContains;
	vector<int> newA, newB;
	for(int i = 0; i < a.size(); i++)
	{
		int which = get_random_int(2);
		bool aPlaced = false, bPlaced = false;
		for (int j = 0; j <= a.size();)
		{
			if (j == a.size())
			{
				j = 0;
				int prevNum = which;
				if (which == 1) which--;
				else if (which == 0) which++;
			}
			int appendToB = which ? a.at(j) : b.at(j);
			int appendToA = which ? b.at(j) : a.at(j);
			if (!aPlaced && aContains.find(appendToA) == aContains.end())
			{
				newA.push_back(appendToA);
				aContains.insert(appendToA);
				aPlaced = true;
			}
			if (!bPlaced && bContains.find(appendToB) == bContains.end())
			{
				newB.push_back(appendToB);
				bContains.insert(appendToB);
				bPlaced = true;
			}
			if (aPlaced && bPlaced) break;
			j++;
		}
	}
	a = newA;
	b = newB;
}

void NQueensSolver::print_chromosome_data(chromosome<int>& data)
{
	int numQueens = 0;
	for (int i = 0; i < data.sequence.size(); i++)
		if (data.sequence[i] != 0) numQueens++;
	string n = to_string(sizeOfProblem);
	cout << "Queens on " << n << "x" << n << " Board is: " << to_string(numQueens) << endl;
	GeneticAlgorithm<int>::print_chromosome_data(data);
}
