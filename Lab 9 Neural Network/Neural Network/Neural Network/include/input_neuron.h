#pragma once
#include <stdafx.h>
#include <neuron.h>

class Input_Neuron : public Neuron
{
private:
protected:
public:
	Input_Neuron(unsigned numOutputConnections);

	~Input_Neuron();

	/// The Input layer Simply normalizes incoming data and passes it through to the next layer
	float process_input(float input) override;
};
