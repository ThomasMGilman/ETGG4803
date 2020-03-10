#include "master_mind_solver.h"

void MasterMind::fitness_function_check(chromosome<int>& c)
{
	c.matches = get_match_count(c.sequence, *toSolveFor);
}
