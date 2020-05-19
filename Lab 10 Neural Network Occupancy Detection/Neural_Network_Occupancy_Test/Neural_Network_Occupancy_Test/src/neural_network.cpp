#include <stdafx.h>
#include <neural_network.h>

NeuralNetwork::NeuralNetwork(unsigned num_inputs, unsigned num_hidden, unsigned num_hidden_layers, unsigned num_outputs, float learning_rate) : mLearningRate(learning_rate)
{
	if (num_inputs == 0 || num_hidden == 0 || num_hidden_layers == 0 || num_outputs == 0)
		throw new std::exception("Missing Layer or Neurons!! Need to provide atleast 1 neuron for each layer, and atleast 1 hidden layer!!");

	// Setup Neurons and Bias'
	for (int i = 0; i < num_inputs + 1; i++)
		mInputNeurons.push_back(new Input_Neuron(num_hidden));

	mHiddenNeuron_Outputs.resize(num_hidden_layers);
	for (int i = 0; i < num_hidden_layers; i++)
	{
		std::vector<Hidden_Neuron*> HiddenLayer;
		mHiddenNeuron_Outputs[i].resize(i < num_hidden_layers - 1 ? num_hidden : num_outputs);
		for (int j = 0; j < num_hidden + 1; j++)
			HiddenLayer.push_back(new Hidden_Neuron(i < num_hidden_layers - 1 ? num_hidden : num_outputs));
		mHiddenLayers.push_back(HiddenLayer);
	}

	for (int i = 0; i < num_outputs; i++)
		mOutputNeurons.push_back(new Output_Neuron());

	// Presize InputVectors for following layers
	mInputNeuron_Outputs.resize(num_hidden);
}

NeuralNetwork::~NeuralNetwork()
{
	for (int i = 0; i < mInputNeurons.size(); i++)
		delete(mInputNeurons[i]);

	for (int i = 0; i < mHiddenLayers.size(); i++)
	{
		std::vector<Hidden_Neuron*>* HiddenLayer = &mHiddenLayers[i];
		for(int j = 0; j < HiddenLayer->size(); j++)
			delete((*HiddenLayer)[j]);
	}

	for (int i = 0; i < mOutputNeurons.size(); i++)
		delete(mOutputNeurons[i]);
}

void NeuralNetwork::set_input_normalization_func(float minVal, float rangeVal)
{
	for (int i = 0; i < mInputNeurons.size(); i++)
		mInputNeurons[i]->set_normalization_func(minVal, rangeVal);
}

void NeuralNetwork::set_input_i_normalization_func(float minVal, float rangeVal, int index)
{
	if (index >= mInputNeurons.size() || index < 0)
		throw new std::exception(("Trying to access Data outside of range of number of inputs!!\n\tIndex Trying To Access: " +
			stringify<int>(mInputNeurons.size()) + "\n\tNumInputs: " + stringify<int>(index)).c_str());

	mInputNeurons[index]->set_normalization_func(minVal, rangeVal);
}

bool NeuralNetwork::changes_occured(bool reset_weights)
{
	bool change_occured = false;
	for (int i = 0; i < mHiddenLayers.size(); i++)
	{
		std::vector<Hidden_Neuron*>* HiddenLayer = &mHiddenLayers[i];
		for (int j = 0; j < HiddenLayer->size(); j++)
			if ((*HiddenLayer)[j]->check_weights_changed(reset_weights)) change_occured = true;
	}

	for (int i = 0; i < mInputNeurons.size(); i++)
		if (mInputNeurons[i]->check_weights_changed(reset_weights)) change_occured = true;

	return change_occured;
}

vector<float> NeuralNetwork::forward_propagate_to_end(std::vector<float> inputs, bool debug_print)
{
	if (inputs.size() != mInputNeurons.size()-1)
		throw new std::exception(("Invalid Number of Inputs!!! Need to provide the same number of inputs as there are input nodes!!\n\tInputs Given: " + 
			stringify<int>(inputs.size()) + " Inputs Expected: " + stringify<int>(mInputNeurons.size())).c_str());

	activate_neurons(&inputs, &mInputNeurons, true);	// Process inputs (does nothing but normalize)
	process_layer<Input_Neuron, Hidden_Neuron>(&mInputNeurons, &mHiddenLayers[0], &mInputNeuron_Outputs, true);	// Process data from input layer to hidden layer
	activate_neurons<Hidden_Neuron>(&mInputNeuron_Outputs, &mHiddenLayers[0], true);	// Activate First Hidden Layer and get Inputs for next layer
	
	for (int i = 0; i < mHiddenLayers.size()-1; i++)
	{
		process_layer<Hidden_Neuron, Hidden_Neuron>(&mHiddenLayers[i], &mHiddenLayers[i+1], &mHiddenNeuron_Outputs[i], true); // Process data from Hidden Layer to next Hidden Layer
		activate_neurons<Hidden_Neuron>(&mHiddenNeuron_Outputs[i], &mHiddenLayers[i+1], true);	//Activate Next Hidden Layer
	}

	process_layer<Hidden_Neuron, Output_Neuron>(&mHiddenLayers[mHiddenLayers.size() - 1], &mOutputNeurons, &mHiddenNeuron_Outputs[mHiddenNeuron_Outputs.size() - 1], false);
	std::vector<float> outputActivations = activate_neurons<Output_Neuron>(&(mHiddenNeuron_Outputs[mHiddenNeuron_Outputs.size()-1]), &mOutputNeurons, false); // Activate Output Layer and get Outputs

	if (debug_print) debug_print_layer_inputs(outputActivations, "Final Outputs");	// Print Outputs

	return outputActivations;
}

void NeuralNetwork::backward_propagate_to_beginning(std::vector<float> outputs, std::vector<float> OL_expectedOutputs, bool update_weights, bool debug_print)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////// Get Errors for Output Layer
	// Calculate Error for Output Layer Nodes and Sum of weights times Error for hidden layer error calc
	for (int i = 0; i < mOutputNeurons.size(); i++)
		mOutputNeurons[i]->calculate_error(OL_expectedOutputs[i], outputs[i]);

	if (debug_print) debug_print_error_dif(&outputs, &OL_expectedOutputs, &mOutputNeurons, "OutputLayer"); // Debug print Output Layer info if enabled

	//////////////////////////////////////////////////////////////////////////////////////////////////////// Get Errors for Hidden Layers and Modify Outgoing Weights
	// Calculate Error for Hidden Layer Nodes and modify each hidden neurons outgoing weight values
	for (int i = mHiddenLayers.size() - 1; i >= 0; i--)
	{
		std::vector<Hidden_Neuron*>* curHL = &mHiddenLayers[i];
		std::vector<float>* HL_Outputs = &mHiddenNeuron_Outputs[i];
		for (int j = 0; j < curHL->size(); j++)
		{
			float weights_times_error = 0;
			if (i != mHiddenLayers.size() - 1)
			{
				for (int k = 0; k < mHiddenLayers[i + 1].size() - 1; k++)
				{
					weights_times_error += (*curHL)[j]->get_weight(k) * mHiddenLayers[i+1][k]->get_error();
					(*curHL)[j]->update_outgoing_weight(k, mHiddenLayers[i+1][k]->get_error(), (*HL_Outputs)[k]);
				}
			}
			else
			{
				for (int k = 0; k < mOutputNeurons.size(); k++)
				{
					weights_times_error += (*curHL)[j]->get_weight(k) * mOutputNeurons[k]->get_error();
					(*curHL)[j]->update_outgoing_weight(k, mOutputNeurons[k]->get_error(), (*HL_Outputs)[k]);
				}
			}
			(*curHL)[j]->calculate_error(weights_times_error);
		}
		if (debug_print) debug_print_error_dif(HL_Outputs, nullptr, curHL, "HiddenLayer_"+stringify<int>(i));	// Debug print Hidden Layer info if enabled
		
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////// Modify Outgoing Weights from Input Layer
	// Modify outgoing weights from Input layer neurons into their corresponding hidden layer neurons
	for (int i = 0; i < mInputNeurons.size(); i++)
	{
		for (int j = 0; j < mHiddenLayers[0].size() - 1; j++)
			mInputNeurons[i]->update_outgoing_weight(j, mHiddenLayers[0][j]->get_error(), mInputNeuron_Outputs[j]);
	}
	if (debug_print) debug_print_error_dif(&mInputNeuron_Outputs, &mInputNeuron_Outputs, &mInputNeurons, "InputLayer");	// Debug print Input Layer info if enabled
}

void NeuralNetwork::debug_print_layer_inputs(std::vector<float> inputs, std::string layerName)
{
	std::cout << stringify<int>(inputs.size()) + " Inputs for Layer: " + layerName << std::endl;
	for (int i = 0; i < inputs.size(); i++)
		std::cout << "\t" + stringify<int>(i) + ": " + stringify<float>(inputs[i]) << std::endl;
	std::cout << std::endl;
}
