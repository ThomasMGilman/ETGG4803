#include <stdafx.h>
#include <output_neuron.h>

Output_Neuron::Output_Neuron() : Neuron(0)
{
}

Output_Neuron::~Output_Neuron()
{
}

float Output_Neuron::calculate_error(float expected_output, float output_val)
{
	mErrorAmnt = (expected_output - output_val) * get_neuron_derivative();
	return mErrorAmnt;
}

float Output_Neuron::process_input(float input)
{
	return Neuron::process_input(input);
}
