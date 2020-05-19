#include <stdafx.h>
#include <neural_network.h>

void print_line_seperator()
{
	std::cout << "----------------------------------------------------------------------------------------" << std::endl;
}

/// Test an input with the given neural network, given what should be expected as outputs to train it. If numTests is 0, train till weights no longer change
void test(NeuralNetwork* nn ,std::vector<float> input, std::vector<float> OL_expectedOut, float minToClassifyAsPos, unsigned numTests, bool debug = false)
{
	int tests = 0;
	int classified_right = 0;

	/// Process inputs until number of tests have been reached if enabled, or until no changes to the internal weights have occurred
	do
	{
		tests++;
		std::vector<float> output = nn->forward_propagate_to_end(input, debug);
		float highestVal = 0;
		for (int i = 0; i < output.size(); i++)
			if (output[i] > highestVal) highestVal = output[i];

		bool posClassification = (highestVal > minToClassifyAsPos);
		if (posClassification == (bool)OL_expectedOut[0]) classified_right++;
		if (debug)
		{
			std::cout << "Test " + stringify<int>(tests) + "\n\tOutput is: { " + convert_to_string<float>(output) + " }"
				+ "\n\t Expected: " + convert_to_string<float>(OL_expectedOut) + "\n\tClassified_right: " + stringify<int>(classified_right) << std::endl;
		}
		nn->backward_propagate_to_beginning(output, OL_expectedOut, true, debug);

	} while (nn->changes_occured(true) && ((numTests <= 0) || (numTests > 0 && tests < numTests)));

	std::cout << "Neural Network Finished Processing Input!!" << std::endl;
	std::cout << "Input: " + convert_to_string<float>(input) + "\nOL_expectedOutputs: { " + convert_to_string<float>(OL_expectedOut) + " }" << std::endl;
	std::cout << "\tNumber of Tests Processed: " + stringify<int>(tests) + "\n\tNumber of Correct Classifications: " + stringify<int>(classified_right) + "\n\tPercentage of Correct Classifications: " + stringify<float>((float)classified_right / (float)tests) << std::endl;
}

void main(int argc, char** argv)
{
	seed_random();

	print_line_seperator();
	std::cout << "OR Neural Network Started!!" << std::endl;
	NeuralNetwork* OR_neur_net = new NeuralNetwork(5, 1, 1, .2f);
	std::vector<float> OR_inputs = { 0, 0, 1, .2, .3 };
	std::vector<float> OR_OL_expected_output = { 1 };
	test(OR_neur_net, OR_inputs, OR_OL_expected_output, 0.51f, 100);
	
	print_line_seperator();
	std::cout << "AND Neural Network Started!!" << std::endl;
	NeuralNetwork* AND_neur_net = new NeuralNetwork(5, 3, 1, .2f);
	std::vector<float> AND_inputs = { 1, 0, .2, .7, 0 };
	std::vector<float> AND_OL_expected_output = {0};
	test(AND_neur_net, AND_inputs, AND_OL_expected_output, 0.51f, 100);
	print_line_seperator();

	std::cout << "NOT AND Neural Network Started!!" << std::endl;
	NeuralNetwork* NAND_neur_net = new NeuralNetwork(5, 3, 1, .2f);
	std::vector<float> NAND_inputs = { 1, 1, .2, .7, 1 };
	std::vector<float> NAND_OL_expected_output = {0};
	test(NAND_neur_net, NAND_inputs, NAND_OL_expected_output, 0.51f, 100);
	print_line_seperator();

	delete(OR_neur_net);
	delete(AND_neur_net);
	delete(NAND_neur_net);
}