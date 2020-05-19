#pragma once

using namespace std;

struct point {
	char character;
	int x;
	int y;
};

enum class L_NORM
{
	ONE_NORM, TWO_NORM
};

template<typename T>
int partition(vector<T>& data, int start, int end, bool (*comparison_func)(T, T))
{
	T pivot = data[end];
	int i = (start - 1);
	for (int j = start; j <= end - 1; j++)
	{
		if (comparison_func(data[j], pivot))
			swap(data[++i], data[j]);
	}
	swap(data[i + 1], data[end]);
	return (i + 1);
};

/// Recursively swaps data from either side of the pivot based on if data is greater or lass than pivot value
template<typename T>
void quickSort(vector<T>& data, int start, int end, bool (*comparison_func)(T, T))
{
	if (start < end)
	{
		int pi = partition<T>(data, start, end, comparison_func);
		quickSort<T>(data, start, pi - 1, comparison_func);
		quickSort<T>(data, pi + 1, end, comparison_func);
	}
};