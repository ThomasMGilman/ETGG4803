#pragma once
#include <string>

class MasterMind {
protected:
private:
	std::string solution;
public:
	/// Initializer, takes argument of type T and converts it into a string to solve for
	template<typename T>
	MasterMind(T toSolveFor);

	template<typename T>
	MasterMind(T* toSolveFor, int size);

	~MasterMind();

	void solver();
};