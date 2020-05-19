#pragma once
#include <stdafx.h>
#include <neuron.h>

class Output_Neuron : public Neuron
{
private:
protected:
public:
	Output_Neuron();

	~Output_Neuron();

	/* Retrieve the amount of error given the expected output of this node, 
	value output by this node, and inpute recieved into this node to produce ouput.
	*/
	float calculate_error(float expected_output, float output_val);

	float process_input(float input) override;
};