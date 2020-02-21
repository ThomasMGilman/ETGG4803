#include "MasterMind.h"
#include <list>

using namespace std;

void main(int argc, char** argv)
{
	const int arraySize = 10;
	int iAData[arraySize] = { 1,2,3,4,5,6,7,8,9,0 };

	MasterMind<int>* masterMindSolver_ia = new MasterMind<int>(iAData, arraySize);
}