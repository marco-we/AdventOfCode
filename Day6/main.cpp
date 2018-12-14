#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
using namespace std;

struct Coord
{
	Coord() : x(0), y(0) {}
	Coord(int _x, int _y) : x(_x), y(_y) {}
	int16_t x;
	int16_t y;
};

int32_t distance(const Coord& a, const Coord& b)
{
	const int32_t x = std::abs(a.x - b.x);
	const int32_t y = std::abs(a.y - b.y);
	return x + y;
}

struct Map
{
	int8_t* closest_coord;
	int8_t* same_dist_counter;
	int32_t* closest_distances;
	int16_t width;
	int16_t height;
	Coord offset;

	unsigned int get_offset(int x, int y) { return width * y + x; }
};

int main(int argc, char** argv)
{
	ifstream file("input.txt");
	if (!file.is_open())
	{
		return EXIT_FAILURE;
	}

	vector<Coord> coords;
	while (!file.eof())
	{
		string s;
		getline(file, s);

		if (file)
		{
			int x, y;
			if (sscanf(s.c_str(), "%d, %d", &x, &y) != 2)
			{
				return EXIT_FAILURE;
			}

			coords.push_back(Coord(x, y));
		}
	}

	Coord min_coord(INT16_MAX, INT16_MAX);
	Coord max_coord(INT16_MIN, INT16_MIN);

	const unsigned int num_coords = (unsigned int)coords.size();
	for (unsigned int i = 0; i < num_coords; ++i)
	{
		Coord curr = coords[i];
		min_coord.x = std::min(min_coord.x, curr.x);
		min_coord.y = std::min(min_coord.y, curr.y);
		max_coord.x = std::max(max_coord.x, curr.x);
		max_coord.y = std::max(max_coord.y, curr.y);
	}

	const Coord delta(std::abs(min_coord.x - max_coord.x), std::abs(min_coord.y - max_coord.y));
#if 0
	min_coord.x -= delta.x;
	min_coord.y -= delta.y;
	max_coord.x += delta.x;
	max_coord.y += delta.y;
#else
	min_coord.x -= 1;
	min_coord.y -= 1;
	max_coord.x += 1;
	max_coord.y += 1;
#endif

	Map m;
	m.width = max_coord.x - min_coord.x;
	m.height = max_coord.y - min_coord.y;
	m.offset = min_coord;
	const unsigned int array_length = m.width * m.height;
	m.closest_coord = new int8_t[array_length];
	m.same_dist_counter = new int8_t[array_length];
	m.closest_distances = new int32_t[array_length];
	memset(m.closest_coord, -1, array_length);
	memset(m.same_dist_counter, 0, array_length);
	for (unsigned int i = 0; i < array_length; ++i)
	{
		m.closest_distances[i] = INT32_MAX;
	}

	for (int16_t y = 0; y < m.height; ++y)
	{
		for (int16_t x=0; x < m.width; ++x)
		{
			const unsigned int idx = m.get_offset(x, y);
			const Coord cur_coord(x + m.offset.x, y + m.offset.y);

			for (unsigned int i = 0; i < num_coords; ++i)
			{
				const int32_t dist = distance(cur_coord, coords[i]);
				if (dist < m.closest_distances[idx])
				{
					m.closest_coord[idx] = i;
					m.closest_distances[idx] = dist;
					m.same_dist_counter[idx] = 0;
				}
				else if (dist == m.closest_distances[idx])
				{
					m.same_dist_counter[idx]++;
				}
			}
		}
	}

	int32_t* candidates = new int32_t[num_coords];
	memset(candidates, 0, num_coords * sizeof(int32_t));

	for (int16_t y = 0; y < m.height; ++y)
	{
		unsigned int idx = m.get_offset(0, y);
		if (m.same_dist_counter[idx] == 0)
		{
			candidates[m.closest_coord[idx]] = -1;
		}

		idx = m.get_offset(m.width-1, y);
		if (m.same_dist_counter[idx] == 0)
		{
			candidates[m.closest_coord[idx]] = -1;
		}
	}

	for (int16_t x = 0; x < m.width; ++x)
	{
		unsigned int idx = m.get_offset(x, 0);
		if (m.same_dist_counter[idx] == 0)
		{
			candidates[m.closest_coord[idx]] = -1;
		}

		idx = m.get_offset(x, m.height - 1);
		if (m.same_dist_counter[idx] == 0)
		{
			candidates[m.closest_coord[idx]] = -1;
		}
	}

	for (int16_t y = 0; y < m.height; ++y)
	{
		for (int16_t x = 0; x < m.width; ++x)
		{
			const unsigned int idx = m.get_offset(x, y);
			int8_t closest = m.closest_coord[idx];
			if (candidates[closest] == -1)
				continue;
			if (m.same_dist_counter[idx] == 0)
			{
				candidates[closest]++;
			}
		}
	}

#if 1
	ofstream debug("output.txt");
	debug << "Offset:    " << m.offset.x << " x " << m.offset.y << endl;
	debug << "Dimension: " << m.width << " x " << m.height << endl << endl;
	debug << "Candidates:" << endl;
	int32_t largest_area = INT32_MIN;
	for (unsigned int i = 0; i < num_coords; ++i)
	{
		debug << " " << i << ": " << candidates[i] << endl;
		largest_area = std::max(largest_area, candidates[i]);
	}
	debug << endl << "Largest area: " << largest_area << endl;
#if 0
	for (int16_t y = 0; y < m.height; ++y)
	{
		debug << y << ":   ";
		for (int16_t x = 0; x < m.width; ++x)
		{
			const unsigned int idx = y * m.width + x;

			if (m.same_dist_counter[idx] > 0)
				debug << " . ";
			else
				debug << " " << (int)m.closest_coord[idx] << " ";
				//debug << " " << (int)m.closest_coord[idx] << "[" << m.closest_distances[idx] << "] ";
		}
		debug << endl;
	}
#endif
#endif

	return EXIT_SUCCESS;
}
