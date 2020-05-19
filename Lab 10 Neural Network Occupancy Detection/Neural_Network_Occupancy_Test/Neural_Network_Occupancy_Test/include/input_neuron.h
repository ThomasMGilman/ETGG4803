#pragma once
#include <stdafx.h>
#include <neuron.h>

class Input_Neuron : public Neuron
{
private:
	float mMin = 0, mRange = 1;
protected:
public:
	Input_Neuron(unsigned numOutputConnections);

	~Input_Neuron();

	void set_normalization_func(float minVal, float rangeVal);

	/// The Input layer Simply normalizes incoming data and passes it through to the next layer
	float process_input(float input) override;
};
