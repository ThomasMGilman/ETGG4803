#include "n_queens_solver.h"

void NQueensSolver::fitness_function_check(chromosome<int>& c)
{
	c.matches = check_queens(c.sequence);
}
