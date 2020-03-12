#include <stdafx.h>
#include "phrase_solver.h"

void PhraseSolver::fitness_function_check(chromosome<char>& c)
{
	c.matches = get_match_difference_offset(c.sequence, *toSolveFor);
}
