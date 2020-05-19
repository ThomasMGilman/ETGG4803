#include <stdafx.h>
#include <occupancy_detector.h>

struct classifications
{
	float learning_percentage;
	int hiddenNeurons;
	int hidden_Layers;
	std::vector<float> classification_percentages;
	float summed_percentages;

	bool operator<(classifications& other) { return summed_percentages < other.summed_percentages; }
};

void main(int argc, char** argv)
{
	seed_random();
	OccupancyDetector od(1000, 5, 1, 0.29, "../Media/Training", "../Media/Tests", false, "..\\Media\\Output");
	/*std::vector<classifications> batch;
	int hL_neurons = 1, hL_layers = 1;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			std::cout << "------------------------------------------------------------------------------------------------\n" << std::endl;
			float percentage = (float)(j + 1) / 100.0f;
			cout << "Learning Percentage: " << percentage << endl;
			OccupancyDetector od(100, hL_neurons + i, hL_layers, percentage, "../Media/Training", "../Media/Tests", false, "..\\Media\\Output");
			std::vector<float> p = od.get_percentage_right();
			float sum = 0;
			for (int k = 0; k < p.size(); k++) 
				sum += p[k];
			batch.push_back({ percentage, hL_neurons + i, hL_layers, p, sum });
			std::cout << "------------------------------------------------------------------------------------------------\n" << std::endl;
		}
	}
	batch = merge_sort<classifications>(batch, true);
	std::cout << "Best Number of Neurons: " << (batch.end() - 1)->hiddenNeurons << "\nBest Number of Hidden Layers" << (batch.end() - 1)->hidden_Layers;
	std::cout << "\nBest Learning: " << (batch.end() - 1)->learning_percentage << "\nBest_Percentages: { " << convert_to_string<float>((batch.end() - 1)->classification_percentages) << "}" << endl;
	*/
}