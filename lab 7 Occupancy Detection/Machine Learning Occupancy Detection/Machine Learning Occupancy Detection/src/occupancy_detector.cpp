#include <stdafx.h>
#include <occupancy_detector.h>

OccupancyDetector::OccupancyDetector(string training_data_folder, string test_data_folder)
{
	this->build_training_data(training_data_folder);
	//this->output_all_to_files();
	this->process_tests(test_data_folder);
}

void OccupancyDetector::process_tests(string tests_data_folder)
{
	vector<string> test_files = get_files_in_directory(tests_data_folder);
	if (test_files.empty())
		throw new exception(("Training data folder is empty or does not exist!! Path given: " + tests_data_folder).c_str());
	
	std::cout << "Processing Test data..." << std::endl;
	int classified_right = 0;
	int occupied = 0;
	int tests = 0;
	for (int i = 0; i < test_files.size(); i++)
	{
		auto startTime = chrono::system_clock::now();
		vector<string> file_lines = file_reader("", test_files[i]);
		
		for (int j = 1; j < file_lines.size(); j++)
		{
			OccupancyTest* newTest = new OccupancyTest(file_lines[j], { ',', '-', ' ', ':' }, { '\"' });
			newTest->mNormalizedAttributes = normalize_var<Attributes>(newTest->mAttributes, mMinTrainAttribVals, mRangeTrainAttribVals);

			bool is_occupied = false;
			
			// Vote Tally Classification Test
			is_occupied = vote_tally_classification(mOcc->mMeanAttribVals, mUnocc->mMeanAttribVals, newTest->mAttributes, mTrainOccWeights, mTrainUnoccWeights);

			// N and kN Neighbour Classification Test
			//is_occupied = nearest_neighbour_classification(mTraining_data, newTest->mNormalizedAttributes, L_NORM::INF_NORM);


			// Bayesian classification
			//is_occupied = bayesian_classification(mIntervalInfo, newTest);

			// IB1
			//is_occupied = instance_based_classification(mTraining_data, newTest->mNormalizedAttributes);

			if (newTest->mAttributes.Occupancy == 1)
			{
				occupied++;
				if (is_occupied) classified_right++;
			}
			else if (!is_occupied)
					classified_right++;

			tests++;

			if (is_occupied)
				mOcc->classified_tests.push_back(newTest);
			else
				mUnocc->classified_tests.push_back(newTest);
		}
		// Stop Clock
		auto endTime = chrono::system_clock::now();
		float elapsed_seconds = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();
		float elapsed_mili = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
		float elapsed = elapsed_seconds + elapsed_mili * .001f;

		string testNum = stringify<int>(i);
		string txt_name = "VOTE_";

		cout << "--------------------------------------------------- TEST SET: "<< testNum <<" ----------------------------------------------" << endl;
		cout << "--------------------------------------------------- STATISTICS -----------------------------------------------" << endl;
		cout << "Occupied: " << occupied << " UnOccupied: " << tests - occupied << " PercentageOccupied: " << stringify<float>((float)occupied / (float)(tests)) << "\n" << endl;
		cout << "Classified Correct: " << classified_right << " Total_tests: " << tests << " Correct Classification percentage: " << stringify<float>((float)classified_right / (float)(tests)) << "\n" << endl;
		cout << "Classified Test Set: " << testNum << " in " << stringify<float>(elapsed) << "seconds" << endl;
		cout << "--------------------------------------------------- DUMPING TO FILES ------------------------------------------" << endl;
		//Dump Test files
		mOcc->dump_test(i, true, elapsed, txt_name);
		cout << "Wrote Occupied classifications Number: " << testNum << endl;
		mUnocc->dump_test(i, true, elapsed, txt_name);
		cout << "Wrote unOccupied classifications Number: " << testNum << endl;
		cout << "--------------------------------------------------- FINISHED OUTPUTING FILES ----------------------------------\n" << endl;

		//Reset
		occupied = 0;
		classified_right = 0;
		tests = 0;
	}
	
	cout << "Finished Classifying Test data!!" << endl;
}

void OccupancyDetector::build_training_data(string training_data_folder)
{
	vector<string> training_files = get_files_in_directory(training_data_folder);
	if (training_files.empty())
		throw new exception(("Training data folder is empty or does not exist!! Path given: " + training_data_folder).c_str());
	
	std::cout << "Loading training data..." << std::endl;
	for (int i = 0; i < training_files.size(); i++)
	{
		std::vector<string> file_lines = file_reader("", training_files[i]);
		for (int j = 1; j < file_lines.size(); j++)
		{
			OccupancyTest* test = new OccupancyTest(file_lines[j], { ',', '-', ' ', ':' }, { '\"' });
			this->mTraining_data.push_back(test);

			// Classify Training Data
			if (test->mAttributes.Occupancy > 0)
				mOcc->append_attributes(test->mAttributes);
			else
				mUnocc->append_attributes(test->mAttributes);

			numTests++;
		}

		std::cout << "\tLoaded: " << training_files[i] << std::endl;
	}
	

	// Sort Classification Attributes and Calculate Means
	std::cout << "Sorting Training Data Attributes..." << std::endl;
	mOcc->sort_attributes();
	mUnocc->sort_attributes();
	std::cout << "Finished Sorting!" << std::endl;

	mSorted_TempValues = merge_sort(concat_vectors<float>(mUnocc->mSortedTrainTemps, mOcc->mSortedTrainTemps), true);
	mSorted_HumidValues = merge_sort(concat_vectors<float>(mUnocc->mSortedTrainHumidities, mOcc->mSortedTrainHumidities), true);
	mSorted_LightValues = merge_sort(concat_vectors<float>(mUnocc->mSortedTrainLightLvls, mOcc->mSortedTrainLightLvls), true);
	mSorted_CO2Values = merge_sort(concat_vectors<float>(mUnocc->mSortedTrainCO2Lvls, mOcc->mSortedTrainCO2Lvls), true);
	mSorted_HumidRatioValues = merge_sort(concat_vectors<float>(mUnocc->mSortedTrainHumidityRatios, mOcc->mSortedTrainHumidityRatios), true);

	// Build Statistical Data
	mMaxTrainAttribVals = Attributes{
		mOcc->mMaxAttribVals.Date >= mUnocc->mMaxAttribVals.Date ? mOcc->mMaxAttribVals.Date : mUnocc->mMaxAttribVals.Date,
		mOcc->mMaxAttribVals.Time >= mUnocc->mMaxAttribVals.Time ? mOcc->mMaxAttribVals.Time : mUnocc->mMaxAttribVals.Time,
		mOcc->mMaxAttribVals.Temperature >= mUnocc->mMaxAttribVals.Temperature ? mOcc->mMaxAttribVals.Temperature : mUnocc->mMaxAttribVals.Temperature,
		mOcc->mMaxAttribVals.Humidity >= mUnocc->mMaxAttribVals.Humidity ? mOcc->mMaxAttribVals.Humidity : mUnocc->mMaxAttribVals.Humidity,
		mOcc->mMaxAttribVals.Light >= mUnocc->mMaxAttribVals.Light ? mOcc->mMaxAttribVals.Light : mUnocc->mMaxAttribVals.Light,
		mOcc->mMaxAttribVals.CO2 >= mUnocc->mMaxAttribVals.CO2 ? mOcc->mMaxAttribVals.CO2 : mUnocc->mMaxAttribVals.CO2,
		mOcc->mMaxAttribVals.HumidityRatio >= mUnocc->mMaxAttribVals.HumidityRatio ? mOcc->mMaxAttribVals.HumidityRatio : mUnocc->mMaxAttribVals.HumidityRatio,
		1
	};
	mMinTrainAttribVals = Attributes{
		mOcc->mMinAttribVals.Date <= mUnocc->mMinAttribVals.Date ? mOcc->mMinAttribVals.Date : mUnocc->mMinAttribVals.Date,
		mOcc->mMinAttribVals.Time <= mUnocc->mMinAttribVals.Time ? mOcc->mMinAttribVals.Time : mUnocc->mMinAttribVals.Time,
		mOcc->mMinAttribVals.Temperature <= mUnocc->mMinAttribVals.Temperature ? mOcc->mMinAttribVals.Temperature : mUnocc->mMinAttribVals.Temperature,
		mOcc->mMinAttribVals.Humidity <= mUnocc->mMinAttribVals.Humidity ? mOcc->mMinAttribVals.Humidity : mUnocc->mMinAttribVals.Humidity,
		mOcc->mMinAttribVals.Light <= mUnocc->mMinAttribVals.Light ? mOcc->mMinAttribVals.Light : mUnocc->mMinAttribVals.Light,
		mOcc->mMinAttribVals.CO2 <= mUnocc->mMinAttribVals.CO2 ? mOcc->mMinAttribVals.CO2 : mUnocc->mMinAttribVals.CO2,
		mOcc->mMinAttribVals.HumidityRatio <= mUnocc->mMinAttribVals.HumidityRatio ? mOcc->mMinAttribVals.HumidityRatio : mUnocc->mMinAttribVals.HumidityRatio,
		0
	}; 
	mMeanTrainAttribVals = Attributes{
		{},{},
		calculate_mean<float>(mSorted_TempValues),
		calculate_mean<float>(mSorted_HumidValues),
		calculate_mean<float>(mSorted_LightValues),
		calculate_mean<float>(mSorted_CO2Values),
		calculate_mean<float>(mSorted_HumidRatioValues)
	};
	mDevTrainAttribVals = Attributes{
		{},{},
		calculate_standard_deviation(mSorted_TempValues, mMeanTrainAttribVals.Temperature),
		calculate_standard_deviation(mSorted_HumidValues, mMeanTrainAttribVals.Humidity),
		calculate_standard_deviation(mSorted_LightValues, mMeanTrainAttribVals.Light),
		calculate_standard_deviation(mSorted_CO2Values, mMeanTrainAttribVals.CO2),
		calculate_standard_deviation(mSorted_HumidRatioValues, mMeanTrainAttribVals.HumidityRatio)
	};

	mRangeTrainAttribVals = mMaxTrainAttribVals - mMinTrainAttribVals;
	mTotalTrainCombinedVals = mOcc->mTotalAttribVals + mUnocc->mTotalAttribVals;
	mTrainOccWeights = normalize_var<Attributes>(mOcc->mMaxAttribVals, mMinTrainAttribVals, mRangeTrainAttribVals);
	mTrainUnoccWeights = normalize_var<Attributes>(mUnocc->mMaxAttribVals, mMinTrainAttribVals, mRangeTrainAttribVals);

	mTrainOccPercentageVals = mOcc->mTotalAttribVals / mTotalTrainCombinedVals;

	for (int i = 0; i < mTraining_data.size(); i++)
		mTraining_data[i]->mNormalizedAttributes = normalize_var<Attributes>(mTraining_data[i]->mAttributes, mMinTrainAttribVals, mRangeTrainAttribVals);

	// Max
	mIntervalInfo.mMaxAttribs = mMaxTrainAttribVals;
	// Min
	mIntervalInfo.mMinAttribs = mMinTrainAttribVals;
	// Means
	mIntervalInfo.mOveralMean = mMeanTrainAttribVals;
	mIntervalInfo.mOccMean = mOcc->mMeanAttribVals;
	mIntervalInfo.mUnoccMean = mUnocc->mMeanAttribVals;
	// Deviation amount
	mIntervalInfo.mOveralDev = mDevTrainAttribVals;
	mIntervalInfo.mOccDev = mOcc->mAttribVariance;
	mIntervalInfo.mUnoccDev = mUnocc->mAttribVariance;
	// Occupied / unoccupied true percentages
	mIntervalInfo.mOccPercentage = mOcc->mTotalAttribVals / mTotalTrainCombinedVals;
	mIntervalInfo.mUnoccPercentage = mUnocc->mTotalAttribVals / mTotalTrainCombinedVals;
	// Set Ranges
	mIntervalInfo.mOveralRanges = mRangeTrainAttribVals;
	mIntervalInfo.mOccRanges = mOcc->mRangeAttribVals;
	mIntervalInfo.mUnoccRanges = mUnocc->mRangeAttribVals;
	//Set normRanges
	mIntervalInfo.mOccRangesNorm = mOcc->mRangeAttribVals / mRangeTrainAttribVals;
	mIntervalInfo.mUnoccRangesNorm = mUnocc->mRangeAttribVals / mRangeTrainAttribVals;

	unsigned interAmount = numTests / 5;
	for (int i = 0; i < 5; i++)
	{
		unsigned j = i % 5;
		unsigned first_index = j * interAmount;
		unsigned second_index = ((j + 1) * interAmount);

		std::pair<float, float> tempPair = { mSorted_TempValues[first_index], mSorted_TempValues[second_index] };
		std::pair<float, float> humidPair = { mSorted_HumidValues[first_index], mSorted_HumidValues[second_index] };
		std::pair<float, float> lightPair = { mSorted_LightValues[first_index], mSorted_LightValues[second_index] };
		std::pair<float, float> co2Pair = { mSorted_CO2Values[first_index], mSorted_CO2Values[second_index] };
		std::pair<float, float> humidRatioPair = { mSorted_HumidRatioValues[first_index], mSorted_HumidRatioValues[second_index] };

		/// Get true percentages of classifications in given paired range specified right above this
		std::pair<float, float> trueProbTemp = { 0, 0 };
		std::pair<float, float> trueProbHumid = { 0, 0 };
		std::pair<float, float> trueProbLight = { 0, 0 };
		std::pair<float, float> trueProbCO2 = { 0, 0 };
		std::pair<float, float> trueProbHumidR = { 0, 0 };
		int TempInRange = 0, HumidInRange = 0, LightInRange = 0, CO2InRange = 0, HumidRInRange = 0;
		for (int i = 0; i < mTraining_data.size(); i++)
		{
			Attributes tA = mTraining_data[i]->mAttributes;
			if (tA.Temperature >= tempPair.first && tA.Temperature <= tempPair.second)
			{
				TempInRange++;
				tA.Occupancy > 0 ? trueProbTemp.first++ : trueProbTemp.second++;
			}
			if (tA.Humidity >= humidPair.first && tA.Humidity <= humidPair.second)
			{
				HumidInRange++;
				tA.Occupancy > 0 ? trueProbHumid.first++ : trueProbHumid.second++;
			}
			if (tA.Light >= lightPair.first && tA.Light <= lightPair.second)
			{
				LightInRange++;
				tA.Occupancy > 0 ? trueProbLight.first++ : trueProbLight.second++;
			}
			if (tA.CO2 >= co2Pair.first && tA.CO2 <= co2Pair.second)
			{
				CO2InRange++;
				tA.Occupancy > 0 ? trueProbCO2.first++ : trueProbCO2.second++;
			}
			if (tA.HumidityRatio >= humidRatioPair.first && tA.HumidityRatio <= humidRatioPair.second)
			{
				HumidRInRange++;
				tA.Occupancy > 0 ? trueProbHumidR.first++ : trueProbHumidR.second++;
			}
		}
		//Convert tally done above to percentage value
		trueProbTemp.first /= TempInRange; trueProbTemp.second /= TempInRange;
		trueProbHumid.first /= HumidInRange; trueProbHumid.second /= HumidInRange;
		trueProbLight.first /= LightInRange; trueProbLight.second /= LightInRange;
		trueProbCO2.first /= CO2InRange; trueProbCO2.second /= CO2InRange;
		trueProbHumidR.first /= HumidRInRange; trueProbHumidR.second /= HumidRInRange;

		//Set part in map
		mIntervalInfo.push_interval_sets({ tempPair, trueProbTemp }, { humidPair, trueProbHumid }, 
											{ lightPair, trueProbLight }, { co2Pair, trueProbCO2 }, 
											{ humidRatioPair, trueProbHumidR });
	}
	mIntervalInfo.debug_print();
	std::cout << "Finished loading training data!\n" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// OUTPUT AND DEBUG FUNCTIONS ////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OccupancyDetector::output_all_to_files()
{
	mOcc->output_to_file();
	mUnocc->output_to_file();
}

void OccupancyDetector::output_to_file(vector<OccupancyTest*> data_set, string file_name, string file_path)
{
	std::cout << "Writing to: " << file_path + "\\" + file_name << std::endl;
	int success = this->file_type_writer(data_set, file_path, file_name, false, true);
	if (success == 0)
		std::cout << "Wrote To file: " + file_path + "\\" + file_name << std::endl;
	else
		std::cout << "Failed to write to: " + file_path + "\\" + file_name << std::endl;
}

int OccupancyDetector::file_type_writer(vector<OccupancyTest*> lines, const string& file_path, const string& file_name, bool write_at_end, bool clear_file)
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
		if (clear_file && file_already_exists)
			output_file->clear();

		if (write_at_end)
			output_file->seekp(ios::end);
		else
		{
			output_file->seekp(ios::beg);
			file_writer(this->file_header, output_file);
		}

		for (int i = 0; i < lines.size(); i++)
			file_writer(lines[i]->test_data_string, output_file);

		output_file->close();
	}
	delete(output_file);
	return return_code;
};
