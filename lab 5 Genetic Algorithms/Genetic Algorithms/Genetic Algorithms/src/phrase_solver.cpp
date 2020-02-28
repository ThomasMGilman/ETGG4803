#include "phrase_solver.h"

PhraseSolver::PhraseSolver(const int& size, const int& samples, const int& toKeep, string phrase, vector<char> sequenceRange)
{
	if (phrase.size() == 0)
		throw new exception("Need to pass a valid phrase to solve for!! Cannot solve for empty phrase");
	phraseToSolveFor = new vector<char>();
	for (char c : phrase)
		phraseToSolveFor->push_back(c);

	sequenceValues = sequenceRange;
	parentsToKeep = toKeep;
	samplesPerGeneration = samples;

	solver();
}

PhraseSolver::~PhraseSolver()
{
	delete(phraseToSolveFor);
}

void PhraseSolver::get_parent(Utilities::chromosome<char>& c, vector<Utilities::chromosome<char>>* lastGenParents, int offset)
{
	utility.get_parent<char>(c, *phraseToSolveFor, lastGenParents, sequenceValues,
		parentsToKeep, offset,
		randomParentRangeChance, randParentChance,
		randomWheelSpinRangeChance, randwheelSpinChance,
		resetRangeChance, resetChance);
}