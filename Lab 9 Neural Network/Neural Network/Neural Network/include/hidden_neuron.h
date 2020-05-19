#pragma once
#include <stdafx.h>
#include <neuron.h>

class Hidden_Neuron : public Neuron
{
private:
protected:
public:
	Hidden_Neuron(unsigned numOutputConnections);

	~Hidden_Neuron();

	/// multiply_with_activated_input (mG_x) which is the result of the logistic function
	float multiply_with_weight(int index) override;

	/// The hidden layer performs the activation function on the incoming input and passes that through to the next layer
	float process_input(float input) override;

	/// The error for this neuron is determined by the der
	float calculate_error(float sum_outputWeights_to_j_times_j_error);
};
