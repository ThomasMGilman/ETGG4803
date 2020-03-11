#include "n_queens_solver.h"

vector<int> NQueensSolver::create_chromosome_data()
{
	vector<int> availableSpaces, data;
	for (int space : sequenceValues)
		availableSpaces.push_back(space);
	for (int i = 0; i < sizeOfProblem; i++)
	{
		int indexChosen = get_random_int(availableSpaces.size());
		data.push_back(availableSpaces.at(indexChosen));
		availableSpaces.erase(availableSpaces.begin() + indexChosen);
	}
	return data;
}

void NQueensSolver::fitness_function_check(chromosome<int>& c)
{
	c.matches = check_queens(c.sequence);
}

void NQueensSolver::multi_point_crossover(vector<int>& a, vector<int>& b, int end, int start)
{
}

void NQueensSolver::uniform_crossover(vector<int>& a, vector<int>& b)
{
}
