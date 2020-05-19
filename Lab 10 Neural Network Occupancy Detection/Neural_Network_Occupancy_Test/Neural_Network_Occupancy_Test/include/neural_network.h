#pragma once
#include <stdafx.h>
#include <Utilities.h>
#include <neuron.h>
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
	std::vector<Input_Neuron*> mInputNeurons;

	std::vector<std::vector<Hidden_Neuron*>> mHiddenLayers;

	std::vector<Output_Neuron*> mOutputNeurons;

	/// Input Vectros. These are the input vectors for layers past the input layer
	std::vector<float> mInputNeuron_Outputs;
	
	std::vector<std::vector<float>> mHiddenNeuron_Outputs;

	float mLearningRate;

protected:
public:
	NeuralNetwork(unsigned num_inputs, unsigned num_hidden, unsigned num_hidden_layers, unsigned num_outputs, float learning_rate);

	/// Clean up and delete all allocated Neurons
	~NeuralNetwork();

	void set_input_normalization_func(float minVal, float rangeVal);

	void set_input_i_normalization_func(float minVal, float rangeVal, int index);

	/// Check to see if any changes to the weights have occured. This should be done after backward_propagation has occured.
	bool changes_occured(bool reset_weights);

	/// Activate Neurons and return a vector of their activation values as inputs to be doted with the weights going into the next layer or otherwise output if output layer.
	template<typename T>
	vector<float> activate_neurons(std::vector<float>* inputs, std::vector<T*>* Neurons, bool containsBias)
	{
		int numNeurons = Neurons->size() - (containsBias ? 1 : 0);
		if (inputs->size() != numNeurons)
		{
			std::string errMsg = ("Mismatched Sizes!! number of inputs must match number of recieving Neurons!!\n\tNumber of Inputs:" +
				stringify<int>(inputs->size()) + "\n\tNumber of Neurons: " + stringify<int>(Neurons->size()));
			std::cout << errMsg << std::endl;
			throw new std::exception(errMsg.c_str());
		}
			
		vector<float> results;
		for (int i = 0; i < numNeurons; i++)
			results.push_back((*Neurons)[i]->process_input((*inputs)[i]));
		return results;
	}

	/// Process Forward Propagation between layers. Get outputs from one layer to be processed into the next given inputs.
	template<typename T, typename O>
	void process_layer(std::vector<T*>* priorLayer, std::vector<O*>* nextLayer, std::vector<float>* nextLayerInputsHolder, bool containsBias)
	{
		int numNextNeurons = nextLayer->size() - (containsBias ? 1 : 0);
		if (nextLayerInputsHolder->size() != numNextNeurons)
		{
			std::string errMsg = ("Mismatched Sizes!! number of recieving Neurons must match the number of Input spaces in inputHolder!!\n\tHolder Size:" +
				stringify<int>(nextLayerInputsHolder->size()) + "\n\tNumber of next layer Neurons: " + stringify<int>(nextLayer->size()));
			std::cout << errMsg << std::endl;
			throw new std::exception(errMsg.c_str());
		}

		for (int i = 0; i < numNextNeurons; i++)
		{
			double i_input_val = 0;
			for (int j = 0; j < priorLayer->size(); j++)
			{
				float input_times_weight_i = (*priorLayer)[j]->multiply_with_weight(i);
				i_input_val += input_times_weight_i;
			}
			(*nextLayerInputsHolder)[i] = i_input_val;
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
	void debug_print_error_dif(std::vector<float>* output, std::vector<float>* expected, std::vector<T*>* neurons, std::string outputtingLayer)
	{
		std::cout << "-----------------------------------------------------------------------------------------------------------------\n" << "Outputting Layer: " + outputtingLayer + " outputs, expected values, logistics, and derivatives" << std::endl;
		for (int i = 0; i < expected->size(); i++)
			std::cout << outputtingLayer + "_Node " + stringify<int>(i) + ":\n\tOutput: " + stringify<float>((*output)[i]) + "\n\tExpected: " + stringify<float>(expected != nullptr ? (*expected)[i] : 0.0) +
			"\n\tOut going WeightsChanged: " + stringify<bool>((*neurons)[i]->are_weights_changed()) +
			"\n\tLogistic: " + stringify<float>((*neurons)[i]->get_activation_output()) + 
			"\n\tDerivative: " + stringify<float>((*neurons)[i]->get_recent_derivative()) +
			"\n\tErrorAmnt: " + stringify<float>((*neurons)[i]->get_error()) +
			"\n-----------------------------------------------------------------------------------------------------------------" << endl;
	};
};