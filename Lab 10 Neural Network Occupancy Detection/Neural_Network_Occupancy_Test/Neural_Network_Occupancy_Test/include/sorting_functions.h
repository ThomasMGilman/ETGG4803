#pragma once
#include <stdafx.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// SORTING FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Determins the pivot index of the incoming data between start and end indexs'
/// Swaps data to either side of pivot based on less than or greater than said value
template<typename T>
int partition(vector<T>& data, int start, int end)
{
	T pivot = data[end];
	int i = (start - 1);
	for (int j = start; j <= end - 1; j++)
	{
		if (data[j] < pivot)
			swap(data[++i], data[j]);
	}
	swap(data[i + 1], data[end]);
	return (i + 1);
};

/// Recursively swaps data from either side of the pivot based on if data is greater or lass than pivot value
template<typename T>
void quickSort(vector<T>& data, int start, int end)
{
	if (start < end)
	{
		int pi = partition(data, start, end);
		quickSort(data, start, pi - 1);
		quickSort(data, pi + 1, end);
	}
};

/// Determins the pivot index of the incoming data between start and end indexs'
/// Swaps data to either side of pivot based on less than or greater than said value
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

/// Spilts up the work recursively, and if enabled will quicksort if the chunck is small enough
/// Returns back a sorted vector of type T.
template<typename T>
vector<T> merge_sort(vector<T> vectorIn, bool useQuickSort = false, int QS_MaxSize = 100)
{
	if (vectorIn.size() > 1)
	{
		int middle = vectorIn.size() / 2;
		vector<T> left_vector(vectorIn.begin(), vectorIn.begin() + middle);
		vector<T> right_vector(vectorIn.begin() + middle, vectorIn.end());

		if (useQuickSort && left_vector.size() <= QS_MaxSize)
			quickSort<T>(left_vector, 0, left_vector.size() - 1);
		else
			left_vector = merge_sort<T>(left_vector, useQuickSort, QS_MaxSize);
		if (useQuickSort && right_vector.size() <= QS_MaxSize)
			quickSort<T>(right_vector, 0, right_vector.size() - 1);
		else
			right_vector = merge_sort<T>(right_vector, useQuickSort, QS_MaxSize);

		unsigned returnIndex = 0, leftIndex = 0, rightIndex = 0;
		while (leftIndex < left_vector.size() && rightIndex < right_vector.size())
			vectorIn[returnIndex++] = left_vector[leftIndex] <  right_vector[rightIndex] ? left_vector[leftIndex++] : right_vector[rightIndex++];

		while (leftIndex < left_vector.size())
			vectorIn[returnIndex++] = left_vector[leftIndex++];

		while (rightIndex < right_vector.size())
			vectorIn[returnIndex++] = right_vector[rightIndex++];
	}
	return vectorIn;
}

/// Spilts up the work recursively, and if enabled will quicksort if the chunck is small enough
/// Returns back a sorted vector of type T.
template<typename T>
vector<T> merge_sort(vector<T> vectorIn, bool (*comparison_func)(T, T), bool useQuickSort = false, int QS_MaxSize = 100)
{
	if (vectorIn.size() > 1)
	{
		int middle = vectorIn.size() / 2;
		vector<T> left_vector(vectorIn.begin(), vectorIn.begin() + middle);
		vector<T> right_vector(vectorIn.begin() + middle, vectorIn.end());

		if (useQuickSort && left_vector.size() <= QS_MaxSize)
			quickSort<T>(left_vector, 0, left_vector.size() - 1, comparison_func);
		else
			left_vector = merge_sort<T>(left_vector, comparison_func, useQuickSort, QS_MaxSize);
		if (useQuickSort && right_vector.size() <= QS_MaxSize)
			quickSort<T>(right_vector, 0, right_vector.size() - 1, comparison_func);
		else
			right_vector = merge_sort<T>(right_vector, comparison_func, useQuickSort, QS_MaxSize);

		unsigned returnIndex = 0, leftIndex = 0, rightIndex = 0;
		while (leftIndex < left_vector.size() && rightIndex < right_vector.size())
			vectorIn[returnIndex++] = comparison_func(left_vector[leftIndex], right_vector[rightIndex]) ? left_vector[leftIndex++] : right_vector[rightIndex++];

		while (leftIndex < left_vector.size())
			vectorIn[returnIndex++] = left_vector[leftIndex++];

		while (rightIndex < right_vector.size())
			vectorIn[returnIndex++] = right_vector[rightIndex++];
	}
	return vectorIn;
}