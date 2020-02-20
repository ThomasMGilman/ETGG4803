#include "MasterMind.h"


void Main(int argc, char** argv)
{
	int iData = 12142141;
	float fData = 1241242.1241f;
	double dData = 1224124124.12412421;
	int iAData[10] = { 1,2,3,4,5,6,7,8,9,0 };
	float fAData[10] = { 1,2,3,4,5,6,7,8,9,0 };
	double dAData[10] = { 1,2,3,4,5,6,7,8,9,0 };
	std::string sData = "ToSolveFor";

	MasterMind* masterMindSolver_i = new MasterMind(iData);
	MasterMind* masterMindSolver_f = new MasterMind(fData);
	MasterMind* masterMindSolver_d = new MasterMind(dData);
	MasterMind* masterMindSolver_ia = new MasterMind(iaData);
	MasterMind* masterMindSolver_fa = new MasterMind(faData);
	MasterMind* masterMindSolver_da = new MasterMind(daData);
	MasterMind* masterMindSolver_s = new MasterMind(sData);
}