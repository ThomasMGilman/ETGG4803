#pragma once
#include <stdafx.h>

class Neuron
{
private:
protected:
	std::vector<float> mWeights;
	float mInput_recieved = 0;
	float mG_x = 0; // g(x) = 1 / (1 + e^(-x))
	float mDerivative = 0;
	float mErrorAmnt = 0;
	bool mWeightsChanged = false;
public:
	float mLearningRate = 0.2; //20% learn rate

	Neuron(unsigned numOutputConnections);

	~Neuron();

	/// Returns true if any of the outgoing weights from this neuron have been modified.
	bool check_weights_changed(bool reset_changed_flag);

	bool are_weights_changed() { return mWeightsChanged; }

	/// Get the number of outgoing edge weights from this Neuron (1 weighted edge for every neuron on the corresponding next layer)
	int get_num_edges() { return mWeights.size(); }

	/// Get activation function value after processing an input
	float get_activation_output() { return mG_x; }

	/// Get the derivative of the input recieved
	float get_recent_derivative() { return mDerivative; }

	/// Get the weight of the requested edge
	float get_weight(int index) { return mWeights[index]; }

	/// Retrieve the last input this Neuron recieved
	float get_input_recieved() { return mInput_recieved; }

	/// Get the amount of error of this neuron. Error is calculated during backpropagation
	float get_error() { return mErrorAmnt; }

	/// Multiply processed input with corresponding output edge (this is done with every node in this layer as the output for neuron at index)
	virtual float multiply_with_weight(int index);

	/* This is Used to update the outgoing weights from both Hidden Layer Neurons and Input Neurons.
		Provided the weight to be modified is given, 
		including the error of the recieving neuron, 
		and the value recieved as input for that neuron.
	*/
	void update_outgoing_weight(int index_weight_to_j, float j_error, float output_to_j);

	virtual float process_input(float input);
	
	virtual float get_neuron_derivative();
};