#include <stdafx.h>
#include "master_mind_solver.h"
#include "phrase_solver.h"
#include "n_queens_solver.h"

void main(int argc, char** argv)
{
	const int toSolveForSize = 10;		// number of digits in problem
	const int samples = 1000;			// number of samples per generation
	const int parentsToKeep = 10;		// number of parents to keep in next generation
	vector<int> sequenceIntRange;
	for (int i = 0; i <= 9; i++)
		sequenceIntRange.push_back(i);

	vector<int>* problem = new vector<int>{ 1,5,3,5,6,8,9,1,4,1};
	cout << "MasterMind:" << endl;
	MasterMind(toSolveForSize, samples, parentsToKeep, sequenceIntRange);
	cout << "\nProblem2:" << endl;
	MasterMind(problem, samples, parentsToKeep, sequenceIntRange, false, false);

	cout << "\nNQueensSolver:" << endl;
	for (int i = 2; i <= 10; i++)
	{
		vector<int> boardSpots;
		for (int j = 0; j <= i; j++)
			boardSpots.push_back(j);
		cout << "BoardSize: " << to_string(i) << endl;
		NQueensSolver(boardSpots.size() - 1, samples, parentsToKeep, boardSpots, false, false, 30);
		cout << endl;
	}

	//string phrase = "To be, or not to be, that is the question.";
	string phrase = "I hate my life, life is pain, life is suffering; to be or not to be, that is the question. Will we ever find the answer to life? That may never be determined....";
	vector<char> sequenceCharRange;
	for (int i = 32; i < 127; i++)
		sequenceCharRange.push_back(i);

	cout << "\nPhraseSolver:" << endl;
	PhraseSolver(phrase.size(), samples, parentsToKeep, phrase, sequenceCharRange, false, true);

	//pip_probability_counter(20000, 1, 6, true);

	std::cout << "\ndone!!" << std::endl;
}