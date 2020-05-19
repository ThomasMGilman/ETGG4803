#include <stdafx.h>
#include <input_neuron.h>

Input_Neuron::Input_Neuron(unsigned numOutputConnections) : Neuron(numOutputConnections)
{
}

Input_Neuron::~Input_Neuron()
{
}

float Input_Neuron::process_input(float input)
{
	Neuron::process_input(input);
	return input;
}
