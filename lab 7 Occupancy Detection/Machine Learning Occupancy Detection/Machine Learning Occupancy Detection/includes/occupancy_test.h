#pragma once
#include <stdafx.h>
#include <Utilities.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// STRUCTS/FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Time {
	int Hour = 0;
	int Minute = 0;
	int Second = 0;
	float fTime = 0;	// hour + min*.01 + sec*.0001
	std::string sTime;

	bool operator ==(Time& other) const { return fTime == other.fTime; }

	bool operator >(Time& other) const { return fTime > other.fTime; }

	bool operator >=(Time& other) const { return (fTime > other.fTime || *this == other); }

	bool operator <(Time& other) const { return fTime < other.fTime; }

	bool operator <=(Time& other) const { return (fTime < other.fTime || *this == other); }

	Time operator +(Time& other)
	{
		int newH = Hour + other.Hour;
		int newM = Minute + other.Minute;
		int newS = Second + other.Second;
		float newfT = fTime + other.fTime;
		std::string newsT = create_time_string(newH, newM, newS);
		return Time{ newH, newM, newS, newfT, newsT };
	}

	void operator += (Time& other) { *this = *this + other;	}

	Time operator -(Time& other)
	{
		int newH = Hour - other.Hour;
		int newM = Minute - other.Minute;
		int newS = Second - other.Second;
		float newfT = fTime - other.fTime;
		std::string newsT = create_time_string(newH, newM, newS);
		return Time{ newH, newM, newS, newfT, newsT };
	}

	void operator -= (Time& other) { *this = *this - other; }

	Time operator * (float other) {
		int newH = Hour * (int)other;
		int newM = Minute * (int)other;
		int newS = Second * (int)other;
		float newfT = fTime * other;
		string newsT = create_time_string(newH, newM, newS);
		return Time{ newH, newM, newS, newfT, newsT };
	}

	void operator *= (float other) { *this = *this * other; }

	Time operator * (Time& other) {
		int newH = Hour * other.Hour;
		int newM = Minute * other.Minute;
		int newS = Second * other.Second;
		float newfT = fTime * other.fTime;
		string newsT = create_time_string(newH, newM, newS);
		return Time{ newH, newM, newS, newfT, newsT };
	}

	void operator *= (Time& other) { *this = *this * other; }

	Time operator / (float other) {
		int newH = other == 0 ? 0 : Hour / other;
		int newM = other == 0 ? 0 : Minute / other;
		int newS = other == 0 ? 0 : Second / other;
		float newfT = other == 0 ? 0 : fTime / other;
		string newsT = create_time_string(newH, newM, newS);
		return Time{ newH, newM, newS, newfT, newsT };
	}

	void operator /= (float other) { *this = *this / other; }

	Time operator / (Time& other) {
		int newH = other.Hour == 0 ? 0 : Hour / other.Hour;
		int newM = other.Minute == 0 ? 0 : Minute / other.Minute;
		int newS = other.Second == 0 ? 0 : Second / other.Second;
		float newfT = other.fTime == 0 ? 0 : fTime / other.fTime;
		string newsT = create_time_string(newH, newM, newS);
		return Time{ newH, newM, newS, newfT, newsT};
	}

	void operator /= (Time& other) { *this = *this / other; }
};

struct Date {
	int Year = 0;
	int Month = 0;
	int Day = 0;
	float fDate = 0; // year + month * .01 + day * .0001
	std::string sDate;

	bool operator ==(Date& other) const { return sDate == other.sDate; }

	bool operator >(Date& other) const { return fDate > other.fDate; }

	bool operator >=(Date& other) const { return (fDate > other.fDate || *this == other); }

	bool operator <(Date& other) const { return fDate < other.fDate; }

	bool operator <=(Date& other) const { return (fDate < other.fDate || *this == other); }

	Date operator +(Date& other)
	{
		int newY = Year + other.Year;
		int newM = Month + other.Month;
		int newD = Day + other.Day;
		float newfD = fDate + other.fDate;
		std::string newsD = create_date_string(newY, newM, newD);
		return Date{ newY, newM, newD, newfD, newsD };
	}

	void operator += (Date& other) { *this = *this + other; }

	Date operator -(Date& other)
	{
		int newY = Year - other.Year;
		int newM = Month - other.Month;
		int newD = Day - other.Day;
		float newfD = fDate - other.fDate;
		std::string newsD = create_date_string(newY, newM, newD);
		return Date{ newY, newM, newD, newfD, newsD };
	}

	void operator -= (Date& other) { *this = *this + other; }

	Date operator * (float other)
	{
		int newY = Year * (int)other;
		int newM = Month * (int)other;
		int newD = Day * (int)other;
		float newfDate = fDate * other;
		string newsDate = create_date_string(newY, newM, newD);
		return Date{ newY, newM, newD, newfDate, newsDate };
	}

	void operator *= (float other) { *this = *this * other; }

	Date operator * (Date& other) {
		int newY = Year * other.Year;
		int newM = Month * other.Month;
		int newD = Day * other.Day;
		float newfDate = fDate * other.fDate;
		string newsDate = create_date_string(newY, newM, newD);
		return Date{ newY, newM, newD, newfDate, newsDate };
	}

	void operator *= (Date& other) { *this = *this * other; }

	Date operator / (float other) {
		int newY = other == 0 ? 0 : Year / other;
		int newM = other == 0 ? 0 : Month / other;
		int newD = other == 0 ? 0 : Day / other;
		float newfDate = other == 0 ? 0 : fDate / other;
		string newsDate = create_date_string(newY, newM, newD);
		return Date{ newY, newM, newD, newfDate, newsDate };
	}

	void operator /= (float other) { *this = *this / other; }

	Date operator / (Date& other) {
		int newY = other.Year == 0 ? 0 : Year / other.Year;
		int newM = other.Month == 0 ? 0 : Month / other.Month;
		int newD = other.Day == 0 ? 0 : Day / other.Day;
		float newfDate = other.fDate == 0 ? 0 : fDate / other.fDate;
		string newsDate = create_date_string(newY, newM, newD);
		return Date{newY, newM, newD, newfDate, newsDate};
	}

	void operator /= (Date& other) { *this = *this / other; }
};

void update_time_string(Time& t);

void update_date_string(Date& d);

struct Attributes {
	Date Date;
	Time Time;
	float Temperature = 0;
	float Humidity = 0;
	float Light = 0;
	float CO2 = 0;
	float HumidityRatio = 0;
	float Occupancy = 0;

	Attributes operator- (Attributes& other) {
		return Attributes{
			Date - other.Date,
			Time - other.Time,
			Temperature - other.Temperature,
			Humidity - other.Humidity,
			Light - other.Light,
			CO2 - other.CO2,
			HumidityRatio - other.HumidityRatio,
				Occupancy
		};
	} 

	void operator-= (Attributes& other) { *this = *this - other; }

	Attributes operator+ (Attributes& other) {
		return Attributes{
			Date + other.Date,
			Time + other.Time,
			Temperature + other.Temperature,
			Humidity + other.Humidity,
			Light + other.Light,
			CO2 + other.CO2,
			HumidityRatio + other.HumidityRatio,
				Occupancy
		};
	}

	void operator+= (Attributes& other) { *this = *this + other; }

	Attributes operator* (float other) {
		return Attributes{
			Date * other,
			Time * other,
			Temperature * other,
			Humidity * other,
			Light * other,
			CO2 * other,
			HumidityRatio * other,
				Occupancy
		};
	}

	void operator*= (float other) { *this = *this * other; }

	Attributes operator* (Attributes& other) {
		return Attributes{
			Date * other.Date,
			Time * other.Time,
			Temperature * other.Temperature,
			Humidity * other.Humidity,
			Light * other.Light,
			CO2 * other.CO2,
			HumidityRatio * other.HumidityRatio,
				Occupancy
		};
	}

	void operator*= (Attributes& other) { *this = *this * other; }

	Attributes operator/ (float other) {
		return Attributes{
			Date / other,
			Time / other,
			other == 0 ? 0 : Temperature / other,
			other == 0 ? 0 : Humidity / other,
			other == 0 ? 0 : Light / other,
			other == 0 ? 0 : CO2 / other,
			other == 0 ? 0 : HumidityRatio / other,
				Occupancy
		};
	}

	void operator/= (float other) { *this = *this / other; }

	Attributes operator/ (Attributes& other) {
		return Attributes{
			Date / other.Date,
			Time / other.Time,
			other.Temperature == 0 ? 0 : Temperature / other.Temperature,
			other.Humidity == 0 ? 0 : Humidity / other.Humidity,
			other.Light == 0 ? 0 : Light / other.Light,
			other.CO2 == 0 ? 0 : CO2 / other.CO2,
			other.HumidityRatio == 0 ? 0 : HumidityRatio / other.HumidityRatio,
				Occupancy
		};
	}

	void operator/= (Attributes& other) { *this = *this / other; }
};

enum class L_NORM
{
	ONE_NORM,
	TWO_NORM,
	INF_NORM
};

Attributes get_abs_of_attrib(Attributes a);

float get_one_norm(Attributes disAttributes);

float get_two_norm(Attributes disAttributes);

float get_infinity_norm(Attributes disAttributes);

string stringify_Attribute(Attributes attrib, bool float_time = false);

struct Interval
{
	float low, high, occPercentage, unoccPercentage;
};

class AttribIntervals
{
public:
	vector<Interval> mTempIntervals;
	vector<Interval> mHumidIntervals;
	vector<Interval> mLightIntervals;
	vector<Interval> mCO2Intervals;
	vector<Interval> mHumidRatioIntervals;
	unsigned interCount = 0;

	// Max
	Attributes mMaxAttribs;
	Attributes mMinAttribs;

	// Ranges
	Attributes mOveralRanges;
	Attributes mOccRanges;
	Attributes mOccRangesNorm;
	Attributes mUnoccRanges;
	Attributes mUnoccRangesNorm;

	// Percentages
	Attributes mOccPercentage;
	Attributes mUnoccPercentage;

	// Means
	Attributes mOveralMean;
	Attributes mOccMean;
	Attributes mUnoccMean;

	// Standard Deviations
	Attributes mOveralDev;
	Attributes mOccDev;
	Attributes mUnoccDev;

	/// Input Variables are pairs of pairs. The first pair is the low and high values, and the second pair is true percentage occupied and unoccupied for the range of the first pair
	void push_interval_sets(pair<pair<float, float>, pair<float, float>> temp, pair<pair<float, float>, pair<float, float>> humid,
							pair<pair<float, float>, pair<float, float>> light, pair<pair<float, float>, pair<float, float>> co2,
							pair<pair<float, float>, pair<float, float>> humidRatio);

	void debug_print();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// OCCUPANCYTEST /////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class OccupancyTest
{
private:
protected:
public:
	/////////////////////////////////////// ATTRIBUTES /////////////////////////////////////////////////
	int mTestNumber = 0;
	Attributes mAttributes;
	Attributes mNormalizedAttributes;
	string test_data_string;

	/////////////////////////////////////// CONSTRUCTOR and DESTRUCTOR /////////////////////////////////
	OccupancyTest(string test_line, unordered_set<char> attrib_delim, unordered_set<char> to_ignore);

	~OccupancyTest() {};

	/////////////////////////////////////// DEBUG FUNCTIONS ////////////////////////////////////////////
	void debug_print_data();

	string reconstruct_test_string();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// VOTING FUNCTIONS ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool bayesian_classification(AttribIntervals classInfo, OccupancyTest* testAttribs);

bool vote_tally_classification(Attributes occupiedMeanAttribs, Attributes unoccupiedMeanAttribs, Attributes testAttribs, Attributes occupiedWeights, Attributes unoccupiedWeights);

bool nearest_neighbour_classification(vector<OccupancyTest*> trainingTests, Attributes normalized_testAttribs, L_NORM norm_type, bool use_k_neighbours = false, int k_neighbours = 5);

float IB1_func(float* v, float* w);

bool instance_based_classification(vector<OccupancyTest*> trainingTests, Attributes normalized_testAttribs);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// COMPARISON FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static inline bool distance_less_comparison(pair<Attributes, float> p1, pair<Attributes, float> p2) { return p1.second < p2.second; }

static inline bool time_greater_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Time.fTime > ot2->mAttributes.Time.fTime; }

static inline bool time_less_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Time.fTime < ot2->mAttributes.Time.fTime; }

static inline bool temperature_greater_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Temperature > ot2->mAttributes.Temperature; }

static inline bool temperature_less_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Temperature < ot2->mAttributes.Temperature; }

static inline bool humidty_greater_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Humidity > ot2->mAttributes.Humidity; }

static inline bool humidty_less_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Humidity < ot2->mAttributes.Humidity; }

static inline bool light_greater_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Light > ot2->mAttributes.Light; }

static inline bool light_less_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Light < ot2->mAttributes.Light; }

static inline bool co2_greater_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.CO2 > ot2->mAttributes.CO2; }

static inline bool co2_less_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.CO2 < ot2->mAttributes.CO2; }

static inline bool humidty_ratio_greater_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.HumidityRatio > ot2->mAttributes.HumidityRatio; }

static inline bool humidty_ratio_less_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.HumidityRatio < ot2->mAttributes.HumidityRatio; }

static inline bool occupancy_greater_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Occupancy > ot2->mAttributes.Occupancy; }

static inline bool occupancy_less_comparison(OccupancyTest* ot1, OccupancyTest* ot2) { return ot1->mAttributes.Occupancy < ot2->mAttributes.Occupancy; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// OCCUPANCYCLASSIFICATION ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class OccupancyClassification
{
public:
	/// Sorted Attributes
	int mNumAttribIndicies = 0;
	vector<Date> mSortedTrainingDates;
	vector<Time> mSortedTrainingTimes;
	vector<float> mSortedTrainTemps;
	vector<float> mSortedTrainHumidities;
	vector<float> mSortedTrainLightLvls;
	vector<float> mSortedTrainCO2Lvls;
	vector<float> mSortedTrainHumidityRatios;
	
	vector<OccupancyTest*> classified_tests;

	Attributes mTotalAttribVals;
	Attributes mAttribVariance;		// Standard Deviation values
	Attributes mMeanAttribVals;
	Attributes mRangeAttribVals;

	Attributes mMaxAttribVals;
	Attributes mMedianAttribVals;
	Attributes mMinAttribVals;

	string file_header = "\t\t\t\tTime\t|\tTemperature in Celsius\t|\tHumidity Percentage\t|\tLight in Lux\t|\tCO2 in ppm\t\t|\tHumidityRatio in kgwater-vapor/kg-air\t|\tOccupied";
	bool mIsOccupied;

private:
	void delete_classified_tests();

public:
	OccupancyClassification(bool Occupied);

	~OccupancyClassification();

	void append_attributes(Attributes a);

	/// Assuming the Data is sorted, will return the properly sorted attributes
	Attributes get_attrib_values_at_index(unsigned index);

	/// Sorts the attribute info from least to greatest
	void sort_attributes();

	/// Set the Attribute Info for this classification
	void set_attribute_info();

	/// Returns the number of classifications that are correct.
	int get_num_classified_right(bool debug_print = false);

	/// Get the persentage of classifications that are correct.
	float get_percentage_classified_right(bool debug_print = true);

	string stringify_values_at_index(int index, bool float_time = false);

	void output_to_file(string path_name = "..\\Media\\Output", string file_name = "Attribute_Statistical_Data", string file_type = ".txt");

	void dump_test(int test_num, bool clear_tests = false, float elapsed = 0, string file_name_extension = "", string file_name = "Test_Attribute_Statistical_Data", string path_name = "..\\Media\\Output", string file_type = ".txt");
};