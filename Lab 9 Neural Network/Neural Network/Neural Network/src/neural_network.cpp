#include <stdafx.h>
#include <neural_network.h>

NeuralNetwork::NeuralNetwork(unsigned num_inputs, unsigned num_hidden, unsigned num_outputs, float learning_rate) : mLearningRate(learning_rate)
{
	// Setup Neurons and Bias'
	for (int i = 0; i < num_inputs; i++)
		mInputNodes.push_back(new Input_Neuron(num_hidden));
	mInputBias = new Bias_Neuron(num_hidden, 1.0);

	for (int i = 0; i < num_hidden; i++)
		mHiddenNodes.push_back(new Hidden_Neuron(num_outputs));
	mHiddenBias = new Bias_Neuron(num_outputs, 1.0);

	for (int i = 0; i < num_outputs; i++)
		mOutputNodes.push_back(new 	Output_Neuron());

	// Presize InputVectors for following layers
	mInputNode_Outputs.resize(num_hidden);
	mHiddenNode_Outputs.resize(num_outputs);
}

NeuralNetwork::~NeuralNetwork()
{
	for (int i = 0; i < mInputNodes.size(); i++)
		delete(mInputNodes[i]);
	delete(mInputBias);

	for (int i = 0; i < mHiddenNodes.size(); i++)
		delete(mHiddenNodes[i]);
	delete(mHiddenBias);

	for (int i = 0; i < mOutputNodes.size(); i++)
		delete(mOutputNodes[i]);
}

bool NeuralNetwork::changes_occured(bool reset_weights)
{
	bool change_occured = false;
	for (int i = 0; i < mHiddenNodes.size(); i++)
		if (mHiddenNodes[i]->check_weights_changed(reset_weights)) change_occured = true;
	if (mHiddenBias->check_weights_changed(reset_weights)) change_occured = true;

	for (int i = 0; i < mInputNodes.size(); i++)
		if (mInputNodes[i]->check_weights_changed(reset_weights)) change_occured = true;
	if (mInputBias->check_weights_changed(reset_weights)) change_occured = true;

	return change_occured;
}

vector<float> NeuralNetwork::forward_propagate_to_end(std::vector<float> inputs, bool debug_print)
{
	if (inputs.size() != mInputNodes.size())
		throw new std::exception(("Invalid Number of Inputs!!! Need to provide the same number of inputs as there are input nodes!!\n\tInputs Given: " + 
			stringify<int>(inputs.size()) + " Inputs Expected: " + stringify<int>(mInputNodes.size())).c_str());

	std::vector<float> inputActivations = activate_neurons(&inputs, &mInputNodes);	// Process inputs (does nothing but normalize)

	process_layer<Input_Neuron, Hidden_Neuron>(&mInputNodes, &mHiddenNodes, mInputBias, &mInputNode_Outputs);	// Process data from input layer to hidden layer

	std::vector<float> hiddenActivations = activate_neurons<Hidden_Neuron>(&mInputNode_Outputs, &mHiddenNodes);	// Activate Hidden Layer and get Inputs for next layer

	process_layer<Hidden_Neuron, Output_Neuron>(&mHiddenNodes, &mOutputNodes, mHiddenBias, &mHiddenNode_Outputs); // Process data from hidden layer to output layer

	std::vector<float> outputActivations = activate_neurons<Output_Neuron>(&mHiddenNode_Outputs, &mOutputNodes); // Activate Output Layer and get Outputs

	if (debug_print) debug_print_layer_inputs(outputActivations, "Final Outputs");	// Print Outputs

	return outputActivations;
}

void NeuralNetwork::backward_propagate_to_beginning(std::vector<float> outputs, std::vector<float> OL_expectedOutputs, bool update_weights, bool debug_print)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////// Get Errors for Output Layer
	// Calculate Error for Output Layer Nodes and Sum of weights times Error for hidden layer error calc
	for (int i = 0; i < mOutputNodes.size(); i++)
		mOutputNodes[i]->calculate_error(OL_expectedOutputs[i], outputs[i]);
		
	if (debug_print) debup_print_error_dif(&outputs, &OL_expectedOutputs, &mOutputNodes, "OutputLayer"); // Debug print Output Layer info if enabled

	//////////////////////////////////////////////////////////////////////////////////////////////////////// Get Errors for Hidden Layer and Modify Outgoing Weights
	// Calculate Error for Hidden Layer Nodes and modify each hidden neurons outgoing weight values
	for (int i = 0; i < mHiddenNodes.size(); i++)
	{
		float weights_out_sum = 0;
		for (int j = 0; j < mOutputNodes.size(); j++)
		{
			weights_out_sum += mHiddenNodes[i]->get_weight(j) * mOutputNodes[j]->get_error();
			mHiddenNodes[i]->update_outgoing_weight(j, mOutputNodes[j]->get_error(), mHiddenNode_Outputs[j]);
		}
	}
	// Modify Hidden layer Bias weights as well
	for (int j = 0; j < mOutputNodes.size(); j++)
		mHiddenBias->update_outgoing_weight(j, mOutputNodes[j]->get_error(), mHiddenNode_Outputs[j]);
	if (debug_print) debup_print_error_dif(&mHiddenNode_Outputs, nullptr, &mHiddenNodes, "HiddenLayer");	// Debug print Hidden Layer info if enabled

	//////////////////////////////////////////////////////////////////////////////////////////////////////// Modify Outgoing Weights from Input Layer
	// Modify outgoing weights from Input layer neurons into their corresponding hidden layer neurons
	for (int i = 0; i < mInputNodes.size(); i++)
	{
		for (int j = 0; j < mHiddenNodes.size(); j++)
			mInputNodes[i]->update_outgoing_weight(j, mHiddenNodes[j]->get_error(), mInputNode_Outputs[j]);
	}
	// Modify Input layer Bias weights as well
	for (int j = 0; j < mHiddenNodes.size(); j++)
		mInputBias->update_outgoing_weight(j, mHiddenNodes[j]->get_error(), mInputNode_Outputs[j]);

	if (debug_print) debup_print_error_dif(&mInputNode_Outputs, &mInputNode_Outputs, &mInputNodes, "InputLayer");	// Debug print Input Layer info if enabled
}

void NeuralNetwork::debug_print_layer_inputs(std::vector<float> inputs, std::string layerName)
{
	std::cout << stringify<int>(inputs.size()) + " Inputs for Layer: " + layerName << std::endl;
	for (int i = 0; i < inputs.size(); i++)
		std::cout << "\t" + stringify<int>(i) + ": " + stringify<float>(inputs[i]) << std::endl;
	std::cout << std::endl;
}
