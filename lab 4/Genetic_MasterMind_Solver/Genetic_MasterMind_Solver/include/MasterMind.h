#pragma once
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>

template<typename T>
class MasterMind
{
private:
	std::stringstream ss;
protected:
	std::string mSolution;

public:
	MasterMind(T solution)
	{
		ss << solution;
		ss >> this->mSolution;
		printf_s("Got Solution to solve for: %s",mSolution);
	}

	void solve()
	{

	}
};