#include <stdafx.h>
#include <input_neuron.h>

Input_Neuron::Input_Neuron(unsigned numOutputConnections) : Neuron(numOutputConnections)
{
}

Input_Neuron::~Input_Neuron()
{
}

void Input_Neuron::set_normalization_func(float minVal, float rangeVal)
{
	mMin = minVal;
	mRange = rangeVal;
}

float Input_Neuron::process_input(float input)
{
	float normalized = 0;
	if(mRange != 0) 
		normalized = (input - mMin) / mRange;
	Neuron::process_input(normalized);
	return normalized;
}
