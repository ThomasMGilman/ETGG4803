#include "master_mind_solver.h"
#include "phrase_solver.h"
#include "n_queens_solver.h"
#include <iostream>

void main(int argc, char** argv)
{
	const int toSolveForSize = 10;		// number of digits in problem
	const int samples = 700;			// number of samples per generation
	const int parentsToKeep = 15;		// number of parents to keep in next generation
	vector<int> sequenceIntRange;
	for (int i = 1; i < 8; i++)
		sequenceIntRange.push_back(i);

	//MasterMind* mm = new MasterMind(toSolveForSize, samples, parentsToKeep, sequenceIntRange);
	//delete(mm);

	string phrase = "To be, or not to be, that is the question.";
	vector<char> sequenceCharRange;
	for (int i = 32; i < 127; i++)
		sequenceCharRange.push_back(i);

	PhraseSolver(phrase.size(), samples, parentsToKeep, phrase, sequenceCharRange);
	
	/*for (int i = 2; i <= 10; i++)
	{
		vector<int> boardSpots;
		for (int j = 0; j <= i; j++)
			boardSpots.push_back(j);
		cout << "BoardSize: " << to_string(i) << endl;
		NQueensSolver(boardSpots.size() - 1, samples, parentsToKeep, boardSpots, 700);
		cout << endl;
	}*/

	std::cout << "\ndone!!" << std::endl;
}