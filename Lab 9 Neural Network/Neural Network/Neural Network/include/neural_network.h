#pragma once
#include <stdafx.h>
#include <Utilities.h>
#include <neuron.h>
#include <bias_neuron.h>
#include <hidden_neuron.h>
#include <input_neuron.h>
#include <output_neuron.h>

/*
inputdata -> 
inputNode -> 
processed data into vector + 1 bias -> 
dot that with weight vector -> 
take output and put into activation function in hidden layer -> 
outputs are dotted with weights+1 of output layer -> 
activation function for output layer -> 
then backpropagation occurs where we adjust weights based on outputs
*/

class NeuralNetwork
{
private:
	/// NeuralNetwork Nodes
	std::vector<Input_Neuron*> mInputNodes;
	Bias_Neuron* mInputBias;

	std::vector<Hidden_Neuron*> mHiddenNodes;
	Bias_Neuron* mHiddenBias;

	std::vector<Output_Neuron*> mOutputNodes;

	/// Input Vectros. These are the input vectors for layers past the input layer
	std::vector<float> mInputNode_Outputs;
	
	std::vector<float> mHiddenNode_Outputs;

	float mLearningRate;

protected:
public:
	NeuralNetwork(unsigned num_inputs, unsigned num_hidden, unsigned num_outputs, float learning_rate);

	/// Clean up and delete all allocated Neurons
	~NeuralNetwork();

	/// Check to see if any changes to the weights have occured. This should be done after backward_propagation has occured.
	bool changes_occured(bool reset_weights);

	/// Activate Neurons and modify the inputs to be the activation values to be doted with the weights going into the next layer or otherwise output if output layer.
	template<typename T>
	void activate_neurons(std::vector<float>* inputs, std::vector<T*>* Neurons, bool modifyInputs)
	{
		if (inputs->size() != Neurons->size())
		{
			std::string errMsg = ("Mismatched Sizes!! number of inputs must match number of recieving Neurons!!\n\tNumber of Inputs:" +
				stringify<int>(inputs->size()) + "\n\tNumber of Neurons: " + stringify<int>(Neurons->size()));
			throw new std::exception(errMsg.c_str());
		}

		for (int i = 0; i < Neurons.size(); i++)
		{
			(*Neurons)[i]->process_input((*inputs)[i]);
			if (modifyInputs) (*inputs)[i] = (*Neurons)[i]->get_activation_output();
		}
	}

	/// Activate Neurons and return a vector of their activation values as inputs to be doted with the weights going into the next layer or otherwise output if output layer.
	template<typename T>
	vector<float> activate_neurons(std::vector<float>* inputs, std::vector<T*>* Neurons)
	{
		if (inputs->size() != Neurons->size())
		{
			std::string errMsg = ("Mismatched Sizes!! number of inputs must match number of recieving Neurons!!\n\tNumber of Inputs:" +
				stringify<int>(inputs->size()) + "\n\tNumber of Neurons: " + stringify<int>(Neurons->size()));
			std::cout << errMsg << std::endl;
			throw new std::exception(errMsg.c_str());
		}
			
		vector<float> results;
		for (int i = 0; i < Neurons->size(); i++)
			results.push_back((*Neurons)[i]->process_input((*inputs)[i]));
		return results;
	}

	/// Process Forward Propagation between layers. Get outputs from one layer to be processed into the next given inputs.
	template<typename T, typename O>
	void process_layer(std::vector<T*>* priorLayer, std::vector<O*>* nextLayer, Bias_Neuron* priorBias, std::vector<float>* nextLayerInputsHolder)
	{
		if (nextLayer->size() != nextLayerInputsHolder->size())
		{
			std::string errMsg = ("Mismatched Sizes!! number of recieving Neurons must match the number of Input spaces in inputHolder!!\n\tHolder Size:" +
				stringify<int>(nextLayerInputsHolder->size()) + "\n\tNumber of next layer Neurons: " + stringify<int>(nextLayer->size()));
			std::cout << errMsg << std::endl;
			throw new std::exception(errMsg.c_str());
		}

		for (int i = 0; i < nextLayer->size(); i++)
		{
			double i_input_val = 0;
			for (int j = 0; j < priorLayer->size(); j++)
			{
				float input_times_weight_i = (*priorLayer)[j]->multiply_with_weight(i);
				i_input_val += input_times_weight_i;
			}
			(*nextLayerInputsHolder)[i] = i_input_val + (priorBias == nullptr ? 0 : priorBias->get_output_bias(i));	// Perform activation function on input
		}
	};

	/// Forward propagate an input sequence through to the end and get the output result
	vector<float> forward_propagate_to_end(std::vector<float> inputs, bool debug_print = false);

	/// Backward propagate back through the nodes, get difference between results and expected, *OPTIONALLY* update Weights
	void backward_propagate_to_beginning(std::vector<float> outputs, std::vector<float> OL_expectedOutputs, bool update_weights, bool debug_print = false);

private:
	/// Print Out a given sequence of inputs or returned values, as well as the name of the layer recieved or retrived from.
	void debug_print_layer_inputs(std::vector<float> inputs, std::string layerName);

	template <typename T>
	void debup_print_error_dif(std::vector<float>* output, std::vector<float>* expected, std::vector<T*>* neurons, std::string outputtingLayer)
	{
		std::cout << "-----------------------------------------------------------------------------------------------------------------\n" << "Outputting Layer: " + outputtingLayer + " outputs, expected values, logistics, and derivatives" << std::endl;
		for (int i = 0; i < expected->size(); i++)
			std::cout << outputtingLayer + "_Node " + stringify<int>(i) + ":\n\tOutput: " + stringify<float>((*output)[i]) + "\n\tExpected: " + (expected != nullptr ? stringify<float>((*expected)[i]) : "N/A") +
			"\n\tOut going WeightsChanged: " + stringify<bool>((*neurons)[i]->are_weights_changed()) +
			"\n\tLogistic: " + stringify<float>((*neurons)[i]->get_activation_output()) + 
			"\n\tDerivative: " + stringify<float>((*neurons)[i]->get_recent_derivative()) +
			"\n\tErrorAmnt: " + stringify<float>((*neurons)[i]->get_error()) +
			"\n-----------------------------------------------------------------------------------------------------------------" << endl;
	};
};