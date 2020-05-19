#pragma once
#include <stdafx.h>
#include <neuron.h>

class Bias_Neuron : public Neuron
{
private:
	float mBias;
protected:
public:
	Bias_Neuron(unsigned numOutputConnections, float amount);

	~Bias_Neuron();

	/// Bias is generally 1, but it can be modified to be a different number
	float get_output_bias(int index) { return mBias * mWeights[index]; };
};
