#include "MasterMind.h"
#include <list>
#include <map>
#include <tuple>

void main(int argc, char** argv)
{
	const int toSolveForSize = 10;
	const int samples = 4;
	const int parentsToKeep = 2;

	MasterMind* mm = new MasterMind(toSolveForSize, samples, parentsToKeep);
	delete(mm);
	std::cout << "done!!" << std::endl;
	//delete(pip_probability_counter(100000, 4, 6, true));
}