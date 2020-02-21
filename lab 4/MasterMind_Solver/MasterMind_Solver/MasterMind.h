#pragma once
#include "Utilities.h"

template<typename T>
class MasterMind {
protected:
private:
	std::string solution;
public:
	/// Initializer, takes argument of type T and converts it into a string to solve for
	
	MasterMind(const T toSolveFor[], int size = 1)
	{
		this->solution = stringify(toSolveFor, size);
		std::cout << "Got Data of type: " << typeid(T).name() << "\n\tData: " << this->solution << "\n"<< std::endl;
	};

	~MasterMind();

	tuple<string, string> roulette_selection(list<string> chromosomes)
	{
		list<float> fitnessScores = new list<float>();
		for(int i = 0; i < chromosomes.size())
	}

	void solver();
};