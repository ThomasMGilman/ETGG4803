#include <stdafx.h>
#include "Utilities.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// UTILITY FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// RANDOM FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_random_int(int max, int min) {
	int num = (rand() % max + min);
	//cout << "got randomNum: " << to_string(num) << endl;
	return num;
}

float get_random_float(float max, float min)
{
	return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// CONVERSION FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<int> convert_strings_to_int(vector<string> strings)
{
	vector<int> numerical_conversion;
	for (int i = 0; i < strings.size(); i++)
		numerical_conversion.push_back(atoi(strings[i].c_str()));
	return numerical_conversion;
}

vector<float> convert_strings_to_float(vector<string> strings)
{
	vector<float> numerical_conversion;
	for (int i = 0; i < strings.size(); i++)
		numerical_conversion.push_back((float)atof(strings[i].c_str()));
	return numerical_conversion;
}

vector<double> convert_strings_to_double(vector<string> strings)
{
	vector<double> numerical_conversion;
	for (int i = 0; i < strings.size(); i++)
		numerical_conversion.push_back(atof(strings[i].c_str()));
	return numerical_conversion;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// STRING FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string create_time_string(int Hour, int Minute, int Second)
{
	return stringify<int>(Hour) + ":" + stringify<int>(Minute) + ":" + stringify<int>(Second);
}

string create_date_string(int Year, int Month, int Day)
{
	return stringify<int>(Year) + "-" + stringify<int>(Month) + "-" + stringify<int>(Day);
}

vector<string> get_sub_strings(string line, unordered_set<char> delimeters, unordered_set<char> to_ignore)
{
	vector<string> sub_strings;
	int line_size = line.size();
	string sub_string = "";
	for (int i = 0; i < line_size; i++)
	{
		if(!to_ignore.count(line[i]) && !delimeters.count(line[i]))
			sub_string += line[i];
		if ((delimeters.count(line[i]) || i == line_size - 1) && sub_string.size() != line_size)
		{
			sub_strings.push_back(sub_string);
			sub_string = "";
		}
	}
	return sub_strings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// STATISTICAL FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<int, PipCount>* pip_probability_counter(int sampleSize, int numDice, int numFaces, bool outputData)
{
	int probabilitySize = pow(numFaces, numDice);
	int sampleSpace = sampleSize;
	std::map<int, PipCount>* pipCount = new std::map<int, PipCount>();
	for (int i = numDice; i <= numDice * numFaces; i++)
		pipCount->emplace(i, PipCount());
	do
	{
		int num = 0;
		for (int i = 0; i < numDice; i++)
			num += get_random_int(6, 1);

		pipCount->at(num).timesRolled += 1;
	} while (sampleSpace--);

	if (outputData)
	{
		std::cout << "standard fair " << numFaces << "-Sided dice with " << numDice << " dice samples and their probability\n\tMaxChance: " << probabilitySize << " SampleSize: " << sampleSize << "\n" << std::endl;
		for (std::map<int, PipCount>::iterator probabilitySamples = pipCount->begin(); probabilitySamples != pipCount->end(); probabilitySamples++)
		{
			probabilitySamples->second.probability = (float)probabilitySamples->second.timesRolled / (float)sampleSize * probabilitySize;
			std::cout << "\tPipCount: " << probabilitySamples->first << "\tCount: " << probabilitySamples->second.timesRolled << "\tProbability: " << probabilitySamples->second.probability / probabilitySize << std::endl;
		}
	}
	return pipCount;
}

/*PR(+|ILL) = Sensitivity | PR(+|WELL) = Specificity
PR(ILL|+) = (PR(+|ILL)PR(ILL)) / (PR(+|ILL)PR(ILL) + PR(+|WELL)PR(WELL)
Ex:
	Sensitivity = 99%	Specificity = 99%	TruePositive = 0.5%
	False Positive = 1% = 1-Specificity
	Well = 1 - TruePositive
	PR(ILL|+) = ((.99)*(.005)) / ((.99)*(.005) + (.01)*(.995))
*/
float bayesian_probability(float sensitivity, float specificity, float testPositive)
{
	float numerator = sensitivity * testPositive;
	float denominator = numerator + (1 - specificity) * (1 - testPositive);
	return numerator / denominator;//(sensitivity * testPositive) / (sensitivity * testPositive + (1 - specificity) * (1 - testPositive));
}

/*
P(A | B) = (P(B | A) * P(A)) / P(B)
P(A | B) = (P(B | A) * P(A)) /( P(B | A)*P(A) + P(B | NOT A)*P(NOT A))
A = Sensitivity
B = Specificity
TruePercentage is always true
P(A | B) = (sensitivity * truePercentage) / ((sensitivty * truePercentage) + (1 - Specificity) * (1 - TruePercentage))
*/
double bayesian_probability(double A, double B, double truePos)
{
	double numerator = A * truePos;
	double denominator = numerator + (1 - B) * (1 - truePos);
	return numerator / denominator;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// FILE FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string append_file_to_path(string file_path, string file_name)
{
	filesystem::path pl = file_path;
	return (pl /= file_name).string();
}

bool file_exists(string file_path, string file_name)
{
	struct stat buffer;
	string full_file_path = file_path.empty() ? file_name : append_file_to_path(file_path, file_name);
	return (stat(full_file_path.c_str(), &buffer) == 0);
}

vector<string> get_files_in_directory(string dir_path)
{
	vector<string> files;
	if (filesystem::exists(dir_path) && filesystem::is_directory(dir_path))
	{
		for (const auto& entry : filesystem::directory_iterator(dir_path))
		{
			if (!entry.is_directory())
				files.push_back(entry.path().string());
		}
	}
	return files;
}

vector<string> file_reader(string file_path, string file_name)
{
	string full_file_path = append_file_to_path(file_path, file_name);
	if (!file_exists("", full_file_path))
		throw new exception(("File Read ERROR!!! File specified does not exists along the current path specified!! path: "+ full_file_path).c_str());

	fstream fs(full_file_path, fstream::in);
	string line;
	vector<string> lines;
	while (getline(fs, line))
		lines.push_back(line);
	fs.close();
	return lines;
}

int file_writer(vector<string> lines, string file_path, string file_name, bool write_at_end, bool clear_file)
{
	string full_file_path = append_file_to_path(file_path, file_name);
	bool file_already_exists = file_exists("", full_file_path);

	ofstream* output_file = new ofstream(full_file_path);
	int return_code = 0;
	if (!output_file->is_open())
	{
		cout << "File Open Error!! Could not create / open the file specified!!! file: " + full_file_path << endl;
		return_code = -1;
	}
	else
	{
		cout << full_file_path << endl;

		if (clear_file && file_already_exists)
			output_file->clear();

		if (write_at_end)
			output_file->seekp(ios::end);

		for (int i = 0; i < lines.size(); i++)
			file_writer(lines[i], output_file);

		output_file->close();

		cout << "Finished Writing to: " + full_file_path << endl;
	}
	delete(output_file);
	return return_code;
}

int file_writer(string line, string file_path, string file_name, bool write_at_end, bool clear_file)
{
	string full_file_path = append_file_to_path(file_path, file_name);
	bool file_already_exists = file_exists("", full_file_path);

	ofstream* output_file = new ofstream(full_file_path);
	int return_code = 0;
	if (!output_file->is_open())
	{
		cout << "File Open Error!! Could not create / open the file specified!!! file: " + full_file_path << endl;
		return_code = -1;
	}
	else
	{
		cout << full_file_path << endl;

		if (clear_file && file_already_exists)
			output_file->clear();

		if (write_at_end)
			output_file->seekp(ios::end);
		file_writer(line, output_file, true, true);

		cout << "Finished Writing to: " + full_file_path << endl;
	}
	delete(output_file);
	return return_code;
}

void file_writer(string line, ofstream* file, bool add_new_line, bool close_after_write)
{
	*file << line + (add_new_line ? "\n" : "");
	if (close_after_write)
		file->close();
}
