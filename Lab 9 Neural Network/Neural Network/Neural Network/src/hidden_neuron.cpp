#include <stdafx.h>
#include <hidden_neuron.h>
#include <Utilities.h>

Hidden_Neuron::Hidden_Neuron(unsigned numOutputConnections) : Neuron(numOutputConnections)
{
}

Hidden_Neuron::~Hidden_Neuron()
{
}

float Hidden_Neuron::multiply_with_weight(int index)
{
	return mG_x * mWeights[index];
}

float Hidden_Neuron::process_input(float input)
{
	mG_x = (float)sigmoid_logistic((double)Neuron::process_input(input));
	return mG_x;
}

float Hidden_Neuron::calculate_error(float sum_outputWeights_to_j_times_j_error)
{
	mErrorAmnt = get_neuron_derivative() * sum_outputWeights_to_j_times_j_error;
	return mErrorAmnt;
}
