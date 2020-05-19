#include <stdafx.h>
#include <neuron.h>
#include <Utilities.h>

Neuron::Neuron(unsigned numOutputConnections)
{
	for (unsigned i = 0; i < numOutputConnections; i++)
		mWeights.push_back(get_random_float());
}

Neuron::~Neuron()
{
}

bool Neuron::check_weights_changed(bool reset_changed_flag)
{
	bool change_occured = mWeightsChanged;
	if (reset_changed_flag) 
		mWeightsChanged = false;
	return change_occured;
}

float Neuron::multiply_with_weight(int index)
{
	return mInput_recieved * mWeights[index];
}

void Neuron::update_outgoing_weight(int index_weight_to_j, float j_error, float output_to_j)
{
	float new_weight_to_j = mWeights[index_weight_to_j] + mLearningRate * j_error * output_to_j;
	if (new_weight_to_j != mWeights[index_weight_to_j]) 
		mWeightsChanged = true;
	mWeights[index_weight_to_j] = new_weight_to_j;
}

float Neuron::process_input(float input)
{
	mInput_recieved = input;
	return mInput_recieved;
}

float Neuron::get_neuron_derivative()
{
	mDerivative = (float)sigmoid_derivative((double)mInput_recieved);
	return mDerivative;
}
