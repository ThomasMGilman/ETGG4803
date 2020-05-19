#include <stdafx.h>
#include <bias_neuron.h>

Bias_Neuron::Bias_Neuron(unsigned numOutputConnections, float biasAmount) : Neuron(numOutputConnections), mBias(biasAmount)
{
}

Bias_Neuron::~Bias_Neuron()
{
}
