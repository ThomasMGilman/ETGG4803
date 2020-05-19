#include <stdafx.h>
#include <Utilities.h>
#include <occupancy_test.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// STRUCTS/FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void update_time_string(Time& t)
{
	t.sTime = create_time_string(t.Hour, t.Minute, t.Second);
}

void update_date_string(Date& d)
{
	d.sDate = create_date_string(d.Year, d.Month, d.Day);
}

Attributes get_abs_of_attrib(Attributes a)
{
	return Attributes{
		{
			abs(a.Date.Year),
			abs(a.Date.Month),
			abs(a.Date.Day),
			fabs(a.Date.fDate),
			""
		},
		{
			abs(a.Time.Hour),
			abs(a.Time.Minute),
			abs(a.Time.Second),
			fabs(a.Time.fTime),
			""
		},
		fabs(a.Temperature),
		fabs(a.Humidity),
		fabs(a.Light),
		fabs(a.CO2),
		fabs(a.HumidityRatio),
		a.Occupancy
	};
}

/// Input Variables are pairs of pairs. The first pair is the low and high values, and the second pair is true percentage occupied and unoccupied for the range of the first pair
void AttribIntervals::push_interval_sets(pair<pair<float, float>, pair<float, float>> temp, pair<pair<float, float>, pair<float, float>> humid,
											pair<pair<float, float>, pair<float, float>> light, pair<pair<float, float>, pair<float, float>> co2,
											pair<pair<float, float>, pair<float, float>> humidRatio)
{
	Attributes low = {
		{0,0,0,0,""},
		{0,0,0,0,""},
		temp.first.first,
		humid.first.first,
		light.first.first,
		co2.first.first,
		humidRatio.first.first,
		0
	};
	Attributes high = {
		{0,0,0,0,""},
		{0,0,0,0,""},
		temp.first.second,
		humid.first.second,
		light.first.second,
		co2.first.second,
		humidRatio.first.second,
		0
	};
	if (interCount > 0)
	{
		if (low.Temperature < mTempIntervals[interCount - 1].high) low.Temperature = mTempIntervals[interCount - 1].high;
		if (low.Humidity < mHumidIntervals[interCount - 1].high) low.Humidity = mHumidIntervals[interCount - 1].high;
		if (low.Light < mLightIntervals[interCount - 1].high) low.Light = mLightIntervals[interCount - 1].high;
		if (low.CO2 < mCO2Intervals[interCount - 1].high) low.CO2 = mCO2Intervals[interCount - 1].high;
		if (low.HumidityRatio < mHumidRatioIntervals[interCount - 1].high) low.HumidityRatio = mHumidRatioIntervals[interCount - 1].high;
	}
	/*
	Attributes lowNorm = (low - mMinAttribs) / mOveralRanges;
	Attributes highNorm = (high - mMinAttribs) / mOveralRanges;

	mTempIntervals.push_back({ low.Temperature, high.Temperature,
		bayesian_probability(mOccRangesNorm.Temperature, lowNorm.Temperature, temp.second.first) * bayesian_probability(mOccRangesNorm.Temperature, highNorm.Temperature, temp.second.first),
		bayesian_probability(mUnoccRangesNorm.Temperature, lowNorm.Temperature, temp.second.second)* bayesian_probability(mUnoccRangesNorm.Temperature, highNorm.Temperature, temp.second.second) });

	mHumidIntervals.push_back({ low.Humidity, high.Humidity,
		bayesian_probability(mOccRangesNorm.Humidity, lowNorm.Humidity, humid.second.first)* bayesian_probability(mOccRangesNorm.Humidity, highNorm.Humidity, humid.second.first),
		bayesian_probability(mUnoccRangesNorm.Humidity, lowNorm.Humidity, humid.second.second)* bayesian_probability(mUnoccRangesNorm.Humidity, highNorm.Humidity, humid.second.second) });

	mLightIntervals.push_back({ low.Light, high.Light,
		bayesian_probability(mOccRangesNorm.Light, lowNorm.Light, light.second.first)* bayesian_probability(mOccRangesNorm.Light, highNorm.Light, light.second.first),
		bayesian_probability(mUnoccRangesNorm.Light, lowNorm.Light, light.second.second)* bayesian_probability(mUnoccRangesNorm.Light, highNorm.Light, light.second.second) });

	mCO2Intervals.push_back({ low.CO2, high.CO2,
		bayesian_probability(mOccRangesNorm.CO2, lowNorm.CO2, co2.second.first)* bayesian_probability(mOccRangesNorm.CO2, highNorm.CO2, co2.second.first),
		bayesian_probability(mUnoccRangesNorm.CO2, lowNorm.CO2, co2.second.second)* bayesian_probability(mUnoccRangesNorm.CO2, highNorm.CO2, co2.second.second) });

	mHumidRatioIntervals.push_back({ low.HumidityRatio, high.HumidityRatio,
		bayesian_probability(mOccRangesNorm.HumidityRatio, lowNorm.HumidityRatio, humidRatio.second.first)* bayesian_probability(mOccRangesNorm.HumidityRatio, highNorm.HumidityRatio, humidRatio.second.first),
		bayesian_probability(mUnoccRangesNorm.HumidityRatio, lowNorm.HumidityRatio, humidRatio.second.second)* bayesian_probability(mUnoccRangesNorm.HumidityRatio, highNorm.HumidityRatio, humidRatio.second.second) });
	*/
	mTempIntervals.push_back({ low.Temperature, high.Temperature, temp.second.first, temp.second.second });

	mHumidIntervals.push_back({ low.Humidity, high.Humidity, humid.second.first, humid.second.second });

	mLightIntervals.push_back({ low.Light, high.Light, light.second.first, light.second.second });

	mCO2Intervals.push_back({ low.CO2, high.CO2, co2.second.first, co2.second.second });

	mHumidRatioIntervals.push_back({ low.HumidityRatio, high.HumidityRatio, humidRatio.second.first, humidRatio.second.second });

	interCount++;
}

void AttribIntervals::debug_print()
{
	cout << "MAP:" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < interCount; i++)
		cout << "\tTemp_interval: " << i << " [" << mTempIntervals[i].low << ", " << mTempIntervals[i].high << "] \t|\tPr(Occupied): " << mTempIntervals[i].occPercentage << "\t|\tPr(Unoccupied): " << mTempIntervals[i].unoccPercentage << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < interCount; i++)
		cout << "\tHumid_interval: " << i << " [" << mHumidIntervals[i].low << ", " << mHumidIntervals[i].high << "] \t|\tPr(Occupied): " << mHumidIntervals[i].occPercentage << "\t|\tPr(Unoccupied): " << mHumidIntervals[i].unoccPercentage << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < interCount; i++)
		cout << "\tLight_interval: " << i << " [" << mLightIntervals[i].low << ", " << mLightIntervals[i].high << "] \t|\tPr(Occupied): " << mLightIntervals[i].occPercentage << "\t|\tPr(Unoccupied): " << mLightIntervals[i].unoccPercentage << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < interCount; i++)
		cout << "\tCO2_interval: " << i << " [" << mCO2Intervals[i].low << ", " << mCO2Intervals[i].high << "] \t|\tPr(Occupied): " << mCO2Intervals[i].occPercentage << "\t|\tPr(Unoccupied): " << mCO2Intervals[i].unoccPercentage << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < interCount; i++)
		cout << "\tHumidRatio_interval: " << i << " [" << mHumidRatioIntervals[i].low << ", " << mHumidRatioIntervals[i].high << "] \t|\tPr(Occupied): " << mHumidRatioIntervals[i].occPercentage << "\t|\tPr(Unoccupied): " << mHumidRatioIntervals[i].unoccPercentage << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
}

float get_one_norm(Attributes disAttributes)
{
	disAttributes = get_abs_of_attrib(disAttributes);
	return disAttributes.Temperature + disAttributes.Humidity + disAttributes.Light + disAttributes.CO2 + disAttributes.HumidityRatio;
}

float get_two_norm(Attributes disAttributes)
{
	return sqrtf(pow(disAttributes.Temperature, 2) + pow(disAttributes.Humidity, 2) + pow(disAttributes.Light, 2) + pow(disAttributes.CO2, 2) + pow(disAttributes.HumidityRatio, 2));
}

float get_infinity_norm(Attributes disAttributes)
{
	float greatestDis = 0;
	disAttributes = get_abs_of_attrib(disAttributes);
	if (disAttributes.Temperature > greatestDis) greatestDis = disAttributes.Temperature;
	if (disAttributes.Humidity > greatestDis) greatestDis = disAttributes.Humidity;
	if (disAttributes.Light > greatestDis) greatestDis = disAttributes.Light;
	if (disAttributes.CO2 > greatestDis) greatestDis = disAttributes.CO2;
	if (disAttributes.HumidityRatio > greatestDis) greatestDis = disAttributes.HumidityRatio;
	return greatestDis;
}

std::string stringify_Attribute(Attributes attrib, bool float_time)
{
	std::string data_string = "";
	data_string += (!float_time ? attrib.Time.sTime : stringify<float>(attrib.Time.fTime)) + "\t|\t";
	data_string += stringify<float>(attrib.Temperature) + " degrees C\t\t|\t\t";
	data_string += stringify<float>(attrib.Humidity) + "%\t\t|\t";
	data_string += stringify<float>(attrib.Light) + " Lux\t|\t";
	data_string += stringify<float>(attrib.CO2) + " ppm\t|\t\t";
	data_string += stringify<float>(attrib.HumidityRatio) + " kgwater-vapor/kg-air\t\t|\t";
	data_string += stringify<float>(attrib.Occupancy);
	return data_string;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// OCCUPANCYTEST /////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OccupancyTest::OccupancyTest(string test_line, unordered_set<char> attrib_delim, unordered_set<char> to_ignore)
{
	vector<string> string_attributes = get_sub_strings(test_line, attrib_delim, to_ignore);
	vector<float> attrib_values = convert_strings_to_float(string_attributes);

	// num, year, month, day, hour, minute, second, temp, humid, light, co2, humid_ratio, occupancy
	if (attrib_values.size() == 13)
	{
		this->mTestNumber = attrib_values[0];

		this->mAttributes.Date = Date{
			(int)attrib_values[1],
			(int)attrib_values[2],
			(int)attrib_values[3],
			attrib_values[1] + attrib_values[2] * .01f + attrib_values[3] * .0001f,
			string_attributes[1] + '-' + string_attributes[2] + '-' + string_attributes[3]
		};

		this->mAttributes.Time = Time{
			(int)attrib_values[4],
			(int)attrib_values[5],
			(int)attrib_values[6],
			attrib_values[4] + attrib_values[5] * .01f + attrib_values[6] * .0001f,
			string_attributes[4] + ':' + string_attributes[5] + ':' + string_attributes[6]
		};

		this->mAttributes.Temperature = attrib_values[7];
		this->mAttributes.Humidity = attrib_values[8];
		this->mAttributes.Light = attrib_values[9];
		this->mAttributes.CO2 = attrib_values[10];
		this->mAttributes.HumidityRatio = attrib_values[11];
		this->mAttributes.Occupancy = attrib_values[12];

		this->test_data_string = reconstruct_test_string();
	}
	else
		throw new exception(("Invalid number of attributes!! Need to have 13 attributes, got: "+stringify<int>(attrib_values.size())).c_str());
}

string OccupancyTest::reconstruct_test_string()
{
	string data_string = stringify<int>(this->mTestNumber) + "\t\t|\t";
	data_string += stringify_Attribute(this->mAttributes);
	return data_string;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// VOTING FUNCTIONS ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool vote_tally_classification(Attributes occupiedMeanAttribs, Attributes unoccupiedMeanAttribs, Attributes testAttribs, Attributes occupiedWeights, Attributes unoccupiedWeights)
{
	float occupiedTally = 0, unoccupiedTally = 0;
	Attributes occupiedDistances = get_abs_of_attrib(occupiedMeanAttribs - testAttribs) * occupiedWeights;
	Attributes unoccupiedDistances = get_abs_of_attrib(unoccupiedMeanAttribs - testAttribs) * unoccupiedWeights;

	occupiedDistances.Temperature < unoccupiedDistances.Temperature ? occupiedTally++ : unoccupiedTally++;
	occupiedDistances.Humidity < unoccupiedDistances.Humidity ? occupiedTally++ : unoccupiedTally++;
	occupiedDistances.Light < unoccupiedDistances.Light ? occupiedTally++ : unoccupiedTally++;
	occupiedDistances.CO2 < unoccupiedDistances.CO2 ? occupiedTally++ : unoccupiedTally++;
	occupiedDistances.HumidityRatio < unoccupiedDistances.HumidityRatio ? occupiedTally++ : unoccupiedTally++;

	if (occupiedTally > unoccupiedTally)
		return true;
	else if (occupiedTally < unoccupiedTally || get_random_int())
		return false;
	else
		return true;
}


bool bayesian_classification(AttribIntervals classInfo, OccupancyTest* test)
{
	float OCC = 1, UNOCC = 1;
	Attributes testAttribs = test->mAttributes;
	Attributes normAttribs = test->mNormalizedAttributes;
	for (int i = 0; i < classInfo.interCount; i++)
	{
		if ((testAttribs.Temperature > classInfo.mTempIntervals[i].low && testAttribs.Temperature < classInfo.mTempIntervals[i].high) || (testAttribs.Temperature > classInfo.mTempIntervals[i].low && i == classInfo.interCount - 1))
		{
			float normalizeL = (classInfo.mTempIntervals[i].low - classInfo.mMinAttribs.Temperature) / classInfo.mOveralRanges.Temperature;
			float normalizeH = (classInfo.mTempIntervals[i].high - classInfo.mMinAttribs.Temperature) / classInfo.mOveralRanges.Temperature;

			float ProbOCC_TempL = bayesian_probability(normalizeL, normAttribs.Temperature, classInfo.mTempIntervals[i].occPercentage);
			float ProbOCC_TempH = bayesian_probability(normalizeH, normAttribs.Temperature, classInfo.mTempIntervals[i].occPercentage);
			OCC *= ProbOCC_TempL * ProbOCC_TempH;

			float ProbUNOCC_TempL = bayesian_probability(normalizeL, normAttribs.Temperature, classInfo.mTempIntervals[i].unoccPercentage);
			float ProbUNOCC_TempH = bayesian_probability(normalizeH, normAttribs.Temperature, classInfo.mTempIntervals[i].unoccPercentage);
			UNOCC *= ProbUNOCC_TempL * ProbUNOCC_TempH;
		}
		if ((testAttribs.Humidity > classInfo.mHumidIntervals[i].low && testAttribs.Humidity < classInfo.mHumidIntervals[i].high) || (testAttribs.Humidity > classInfo.mHumidIntervals[i].low && i == classInfo.interCount - 1))
		{
			float normalizeL = (classInfo.mHumidIntervals[i].low - classInfo.mMinAttribs.Humidity) / classInfo.mOveralRanges.Humidity;
			float normalizeH = (classInfo.mHumidIntervals[i].high - classInfo.mMinAttribs.Humidity) / classInfo.mOveralRanges.Humidity;

			float ProbOCC_HumidL = bayesian_probability(normalizeL, normAttribs.Humidity, classInfo.mHumidIntervals[i].occPercentage);
			float ProbOCC_HumidH = bayesian_probability(normalizeH, normAttribs.Humidity, classInfo.mHumidIntervals[i].occPercentage);
			OCC *= ProbOCC_HumidL * ProbOCC_HumidH;

			float ProbUNOCC_HumidL = bayesian_probability(normalizeL, normAttribs.Humidity, classInfo.mHumidIntervals[i].unoccPercentage);
			float ProbUNOCC_HumidH = bayesian_probability(normalizeH, normAttribs.Humidity, classInfo.mHumidIntervals[i].unoccPercentage);
			UNOCC *= ProbUNOCC_HumidL * ProbUNOCC_HumidH;
		}
		if ((testAttribs.Light > classInfo.mLightIntervals[i].low && testAttribs.Light < classInfo.mLightIntervals[i].high) || (testAttribs.Light > classInfo.mLightIntervals[i].low && i == classInfo.interCount - 1))
		{
			float normalizeL = (classInfo.mLightIntervals[i].low - classInfo.mMinAttribs.Light) / classInfo.mOveralRanges.Light;
			float normalizeH = (classInfo.mLightIntervals[i].high - classInfo.mMinAttribs.Light) / classInfo.mOveralRanges.Light;

			float ProbOCC_LightL = bayesian_probability(normalizeL, normAttribs.Light, classInfo.mLightIntervals[i].occPercentage);
			float ProbOCC_LightH = bayesian_probability(normalizeH, normAttribs.Light, classInfo.mLightIntervals[i].occPercentage);
			OCC *= ProbOCC_LightL * ProbOCC_LightH;

			float ProbUNOCC_LightL = bayesian_probability(normalizeL, normAttribs.Light, classInfo.mLightIntervals[i].unoccPercentage);
			float ProbUNOCC_LightH = bayesian_probability(normalizeH, normAttribs.Light, classInfo.mLightIntervals[i].unoccPercentage);
			UNOCC *= ProbUNOCC_LightL * ProbUNOCC_LightH;
		}
		if ((testAttribs.CO2 > classInfo.mCO2Intervals[i].low && testAttribs.CO2 < classInfo.mCO2Intervals[i].high) || (testAttribs.CO2 > classInfo.mCO2Intervals[i].low && i == classInfo.interCount - 1))
		{
			float normalizeL = (classInfo.mCO2Intervals[i].low - classInfo.mMinAttribs.CO2) / classInfo.mOveralRanges.CO2;
			float normalizeH = (classInfo.mCO2Intervals[i].high - classInfo.mMinAttribs.CO2) / classInfo.mOveralRanges.CO2;

			float ProbOCC_CO2L = bayesian_probability(normalizeL, normAttribs.CO2, classInfo.mCO2Intervals[i].occPercentage);
			float ProbOCC_CO2H = bayesian_probability(normalizeH, normAttribs.CO2, classInfo.mCO2Intervals[i].occPercentage);
			OCC *= ProbOCC_CO2L * ProbOCC_CO2H;

			float ProbUNOCC_CO2L = bayesian_probability(normalizeL, normAttribs.CO2, classInfo.mCO2Intervals[i].unoccPercentage);
			float ProbUNOCC_CO2H = bayesian_probability(normalizeH, normAttribs.CO2, classInfo.mCO2Intervals[i].unoccPercentage);
			UNOCC *= ProbUNOCC_CO2L * ProbUNOCC_CO2H;
		}
		if ((testAttribs.HumidityRatio > classInfo.mHumidRatioIntervals[i].low && testAttribs.HumidityRatio < classInfo.mHumidRatioIntervals[i].high) || (testAttribs.HumidityRatio > classInfo.mHumidRatioIntervals[i].low && i == classInfo.interCount - 1))
		{
			float normalizeL = (classInfo.mHumidRatioIntervals[i].low - classInfo.mMinAttribs.HumidityRatio) / classInfo.mOveralRanges.HumidityRatio;
			float normalizeH = (classInfo.mHumidRatioIntervals[i].high - classInfo.mMinAttribs.HumidityRatio) / classInfo.mOveralRanges.HumidityRatio;

			float ProbOCC_HumidRL = bayesian_probability(normalizeL, normAttribs.HumidityRatio, classInfo.mHumidRatioIntervals[i].occPercentage);
			float ProbOCC_HumidRH = bayesian_probability(normalizeH, normAttribs.HumidityRatio, classInfo.mHumidRatioIntervals[i].occPercentage);
			OCC *= ProbOCC_HumidRL * ProbOCC_HumidRH;

			float ProbUNOCC_HumidRL = bayesian_probability(normalizeL, normAttribs.HumidityRatio, classInfo.mHumidRatioIntervals[i].unoccPercentage);
			float ProbUNOCC_HumidRH = bayesian_probability(normalizeH, normAttribs.HumidityRatio, classInfo.mHumidRatioIntervals[i].unoccPercentage);
			UNOCC *= ProbUNOCC_HumidRL * ProbUNOCC_HumidRH;
		}
	}

	return OCC > UNOCC;
}

bool nearest_neighbour_classification(vector<OccupancyTest*> trainingTests, Attributes normalized_testAttribs, L_NORM norm_type, bool use_k_neighbours, int k_neighbours)
{
	vector<std::pair<Attributes, float>> kClosest;
	int num_k_allowed = use_k_neighbours ? k_neighbours : 1;

	for (int i = 0; i < trainingTests.size(); i++)
	{
		Attributes distances = trainingTests[i]->mNormalizedAttributes - normalized_testAttribs;
		float distance = 0;
		switch (norm_type)
		{
		case L_NORM::ONE_NORM:
			distance = get_one_norm(distances);
			break;
		case L_NORM::TWO_NORM:
			distance = get_two_norm(distances);
			break;
		case L_NORM::INF_NORM:
			distance = get_infinity_norm(distances);
			break;
		}
		if (kClosest.empty() || kClosest.size() < num_k_allowed)
		{
			kClosest.push_back({ trainingTests[i]->mAttributes, distance });
			if (kClosest.size() == num_k_allowed && num_k_allowed > 1)
				quickSort<std::pair<Attributes, float>>(kClosest, 0, num_k_allowed - 1, distance_less_comparison);
		}
		else
		{
			std::pair<Attributes, float> closer = { trainingTests[i]->mAttributes, distance };
			for (int j = 0; j < num_k_allowed; j++)
			{
				if (closer.second < kClosest[j].second)	// Swap
				{
					auto tmp = kClosest[j];
					kClosest[j] = closer;
					closer = tmp;
				}
			}
		}
	}
	int occupied_tally = 0, unoccupied_tally = 0;
	for (int i = 0; i < kClosest.size(); i++)
		kClosest[i].first.Occupancy != 0 ? occupied_tally++ : unoccupied_tally++;

	if (occupied_tally > unoccupied_tally) return true;
	else if (occupied_tally < unoccupied_tally) return false;
	else return get_random_int();	// Returns randomly a 0 or 1
}

float IB1_func(float* v, float* w)
{
	float dis = 0;
	if (v != nullptr && w != nullptr) dis = pow(*v - *w, 2);
	else if (v == nullptr && w == nullptr) dis = 1;
	else {
		if (v != nullptr) dis = fabs(*v);
		else dis = fabs(*w);
	}
	return dis;
}

bool instance_based_classification(vector<OccupancyTest*> trainingTests, Attributes normalized_testAttribs)
{
	OccupancyTest* closest = trainingTests[0];
	float dis = INFINITY;

	for (int i = 0; i < trainingTests.size(); i++)
	{
		Attributes distances = trainingTests[i]->mNormalizedAttributes - normalized_testAttribs;
		float distance = get_two_norm(distances);

		if (distance < dis)
		{
			closest = trainingTests[i];
			dis = distance;
		}
	}

	return closest->mAttributes.Occupancy == 1;
}

void OccupancyTest::debug_print_data()
{
	printf_s("Occupancy Test: \d\n\tDate: \d-\d-\d \d:\d:\d\n\tTemperature: \f\n\tHumidity: \f\n\tLight: \f\n\tCO2: \f\n\tHumidityRatio: \f\n\tOccupied: \d\n",
		this->mTestNumber, this->mAttributes.Date.Year, this->mAttributes.Date.Month, this->mAttributes.Date.Day, this->mAttributes.Time.Hour, this->mAttributes.Time.Minute, this->mAttributes.Time.Second,
		this->mAttributes.Temperature, this->mAttributes.Humidity, this->mAttributes.Light, this->mAttributes.CO2, this->mAttributes.HumidityRatio, this->mAttributes.Occupancy);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// OCCUPANCYCLASSIFICATION ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OccupancyClassification::delete_classified_tests()
{
	for (int i = 0; i < classified_tests.size(); i++)
		delete(classified_tests[i]);
	classified_tests.clear();
}

OccupancyClassification::OccupancyClassification(bool Occupied) : mIsOccupied(Occupied)
{
}

OccupancyClassification::~OccupancyClassification()
{
	delete_classified_tests();
}

void OccupancyClassification::append_attributes(Attributes a)
{
	mSortedTrainingDates.push_back(a.Date);
	mSortedTrainingTimes.push_back(a.Time);
	mSortedTrainTemps.push_back(a.Temperature);
	mSortedTrainHumidities.push_back(a.Humidity);
	mSortedTrainLightLvls.push_back(a.Light);
	mSortedTrainCO2Lvls.push_back(a.CO2);
	mSortedTrainHumidityRatios.push_back(a.HumidityRatio);

	mTotalAttribVals += a;
	mNumAttribIndicies++;
}

Attributes OccupancyClassification::get_attrib_values_at_index(unsigned index)
{
	return Attributes{
		mSortedTrainingDates[index],
		mSortedTrainingTimes[index],
		mSortedTrainTemps[index],
		mSortedTrainHumidities[index],
		mSortedTrainLightLvls[index],
		mSortedTrainCO2Lvls[index],
		mSortedTrainHumidityRatios[index],
		(float)mIsOccupied
	};
}

void OccupancyClassification::sort_attributes()
{
	mSortedTrainingDates = merge_sort(mSortedTrainingDates, true);
	mSortedTrainingTimes = merge_sort(mSortedTrainingTimes, true);
	mSortedTrainTemps = merge_sort(mSortedTrainTemps, true);
	mSortedTrainHumidities = merge_sort(mSortedTrainHumidities, true);
	mSortedTrainLightLvls = merge_sort(mSortedTrainLightLvls, true);
	mSortedTrainCO2Lvls = merge_sort(mSortedTrainCO2Lvls, true);
	mSortedTrainHumidityRatios = merge_sort(mSortedTrainHumidityRatios, true);
	set_attribute_info();
}

void OccupancyClassification::set_attribute_info()
{
	mMaxAttribVals = get_attrib_values_at_index(mNumAttribIndicies - 1);
	mMedianAttribVals = get_attrib_values_at_index(mNumAttribIndicies / 2);
	mMinAttribVals = get_attrib_values_at_index(0);

	mMeanAttribVals = mTotalAttribVals / (float)mNumAttribIndicies;
	mRangeAttribVals = mMaxAttribVals - mMinAttribVals;

	// Calculate Standard Deviation
	mAttribVariance.Temperature = calculate_standard_deviation<float>(mSortedTrainTemps, mMedianAttribVals.Temperature);
	mAttribVariance.Humidity = calculate_standard_deviation<float>(mSortedTrainTemps, mMedianAttribVals.Humidity);
	mAttribVariance.Light = calculate_standard_deviation<float>(mSortedTrainTemps, mMedianAttribVals.Light);
	mAttribVariance.CO2 = calculate_standard_deviation<float>(mSortedTrainTemps, mMedianAttribVals.CO2);
	mAttribVariance.HumidityRatio = calculate_standard_deviation<float>(mSortedTrainTemps, mMedianAttribVals.HumidityRatio);
}

int OccupancyClassification::get_num_classified_right(bool debug_print)
{
	int correct = 0;
	for (int i = 0; i < classified_tests.size(); i++)
		correct += (bool)classified_tests[i]->mAttributes.Occupancy == this->mIsOccupied;
	if (debug_print)
	{
		cout << "Occupancy " << this->mIsOccupied << " results:" << endl;
		cout << "\tCorrect: " + stringify<int>(correct) << " Total_Classified: " << stringify<int>(classified_tests.size()) << endl;
	}

	return correct;
}

float OccupancyClassification::get_percentage_classified_right(bool debug_print)
{
	float percentage_right = (float)get_num_classified_right(debug_print) / (float)classified_tests.size();
	if (debug_print)
		cout << "\tPercentage Right: " + stringify<float>(percentage_right) << endl;
	return percentage_right;
}

string OccupancyClassification::stringify_values_at_index(int index, bool float_time)
{
	std::string data_string = "";
	data_string += (!float_time ? mSortedTrainingTimes[index].sTime : stringify<double>(mSortedTrainingTimes[index].fTime)) + "\t|\t";
	data_string += stringify<float>(mSortedTrainTemps[index]) + " degrees C\t\t|\t\t";
	data_string += stringify<float>(mSortedTrainHumidities[index]) + "%\t\t|\t";
	data_string += stringify<float>(mSortedTrainLightLvls[index]) + " Lux\t|\t";
	data_string += stringify<float>(mSortedTrainCO2Lvls[index]) + " ppm\t|\t\t";
	data_string += stringify<float>(mSortedTrainHumidityRatios[index]) + " kgwater-vapor/kg-air\t\t|\t";
	data_string += stringify<bool>(mIsOccupied);
	return data_string;
}

void OccupancyClassification::output_to_file(std::string path_name, std::string file_name, string file_type)
{
	vector<string> combined_attribute_data = {
		"\t" + this->file_header,
		"Classification: "+ mIsOccupied ? "Occupied" : "unOccupied\n",
		"\tTotal Attribute Values:\t\t " + stringify_Attribute(mTotalAttribVals, true),
		"\tMax Attribute Values:\t\t " + stringify_Attribute(mMaxAttribVals, true),
		"\tMedian Attribute Values:\t " + stringify_Attribute(mMedianAttribVals, true),
		"\tMin Attribute Values:\t\t " + stringify_Attribute(mMinAttribVals, true),
		"\tMean Attribute Values:\t\t " + stringify_Attribute(mMeanAttribVals, true),
		"\tRange Attribute Values:\t\t " + stringify_Attribute(mRangeAttribVals, true)
	};
	file_writer(combined_attribute_data, path_name, ((this->mIsOccupied ? "Occupied_" : "Unoccupied_") + file_name + file_type), false, true);
}

void OccupancyClassification::dump_test(int test_num, bool clear_tests, float elapsed, string file_name_extension, string file_name, string path_name, string file_type)
{
	string file_output = "";
	file_output += "Occupancy " + stringify<int>(this->mIsOccupied) + " results:\n";
	file_output += "\tCorrect: " + stringify<int>(get_num_classified_right()) + " Total_Classified: " + stringify<int>(classified_tests.size())+"\n";
	file_output += "\tPercentage Right: " + stringify<float>(get_percentage_classified_right()) + "\n";
	file_output += "Classification Time Taken: " + stringify<float>(elapsed) + "seconds\n";

	file_output += this->file_header + "\n";
	for (int i = 0; i < classified_tests.size(); i++)
		file_output += classified_tests[i]->reconstruct_test_string() + "\n";

	file_writer(file_output, path_name, ((mIsOccupied ? "Occupied_" : "Unoccupied_") + file_name_extension + file_name + "_" + stringify<int>(test_num) + file_type), false, true);

	if (clear_tests)
		delete_classified_tests();
}
