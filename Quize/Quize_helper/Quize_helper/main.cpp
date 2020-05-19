#include <math.h>
#include <vector>
#include <iostream>
#include "sorter.h"

using namespace std;

bool distance_less_comparison(pair<float, point> p1, pair<float, point> p2)
{
	return p1.first < p2.first;
}

float get_one_norm(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

float get_two_norm(int x1, int y1, int x2, int y2)
{
	return sqrtf(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

vector<pair<float, point>> nearest_neighbour_classification(vector<point> points, point m, L_NORM norm_type, bool use_k_neighbours, int k_neighbours)
{
	vector<pair<float, point>> kClosest;
	int num_k_allowed = use_k_neighbours ? k_neighbours : 1;

	for (int i = 0; i < points.size(); i++)
	{
		float distance = 0;
		if (points[i].character == m.character)
			continue;

		switch (norm_type)
		{
		case L_NORM::ONE_NORM:
			distance = get_one_norm(m.x, m.y, points[i].x, points[i].y);
			break;
		case L_NORM::TWO_NORM:
			distance = get_two_norm(m.x, m.y, points[i].x, points[i].y);
			break;
		}
		if (kClosest.empty() || kClosest.size() < num_k_allowed)
		{
			kClosest.push_back({ distance, points[i] });
			if (kClosest.size() == num_k_allowed && num_k_allowed > 1)
				quickSort<pair<float, point>>(kClosest, 0, num_k_allowed - 1, distance_less_comparison);
		}
		else
		{
			std::pair<float, point> closer = { distance, points[i]};
			for (int j = 0; j < num_k_allowed; j++)
			{
				if (closer.first < kClosest[j].first || (closer.second.character < kClosest[j].second.character && closer.first == kClosest[j].first))	// Swap
				{
					auto tmp = kClosest[j];
					kClosest[j] = closer;
					closer = tmp;
				}
			}
		}
	}
	return kClosest;
}

void main()
{
	vector<point> points = {
		{'A', 0, 0}, {'B', 1, 0}, {'C', 2, 0}, {'D', 3, 0}, {'E', 4, 0},
		{'F', 0, 1}, {'G', 1, 1}, {'H', 2, 1}, {'I', 3, 1}, {'J', 4, 1},
		{'K', 0, 2}, {'L', 1, 2}, {'M', 2, 2}, {'N', 3, 2}, {'O', 4, 2},
		{'P', 0, 3}, {'Q', 1, 3}, {'R', 2, 3}, {'S', 3, 3}, {'T', 4, 3},
		{'U', 0, 4}, {'V', 1, 4}, {'W', 2, 4}, {'X', 3, 4}, {'Y', 4, 4},
	};

	point m = { 'M', 2, 2 };

	vector<pair<float, point>> nearest = nearest_neighbour_classification(points, m, L_NORM::ONE_NORM, true, 7);

	for (int i = 0; i < nearest.size(); i++)
		cout << "Neighbour:"<< nearest[i].second.character <<" [" << nearest[i].second.x << ", " << nearest[i].second.y << "] dis: "<< nearest[i].first <<endl;
}