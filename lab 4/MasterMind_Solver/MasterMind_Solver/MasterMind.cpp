#include "MasterMind.h"
#include <iostream>

template<typename T>
MasterMind::MasterMind(T toSolveFor)
{
	this->solution = typeid(T).hash_code() != typeid(std::string).hash_code() ? std::to_string(toSolveFor) : toSolveFor;
	std::cout << "Got Data of type: " << typeid(T).name() << "\n\tData: " << toSolveFor << std::endl;
}

template<typename T>
MasterMind::MasterMind(T* toSolveFor, int size)
{
	//if (typeid(T).hash_code() == typeid(std::string).hash_code())
	std::stringstream ss;
	for (int i = 0; i < size; i++)
	{
		ss << toSolveFor[i];
		if (i < size - 1)
			ss << " ";
	}
		
	ss >> this->solution;
	std::cout << "Got Data of type: " << typeid(T).name() << "\n\tData: " << toSolveFor << std::endl;
}


MasterMind::~MasterMind()
{

}

void MasterMind::solver()
{

}