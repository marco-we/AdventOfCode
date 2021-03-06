#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
using namespace std;

const int NUM_CHARS = 26;

struct Row
{
	char chars[NUM_CHARS];
};

int count_occurrences(Row row, int num)
{
	for (int i = 0; i < NUM_CHARS - 1; ++i)
	{
		const char c = row.chars[i];
		int count = 0;
		for (int j = 0; j < NUM_CHARS; ++j)
		{
			if (row.chars[j] == c)
				count++;
		}
		if (count == num)
			return 1;
	}
	return 0;
}

int hamming_distance(Row a, Row b)
{
	int dist = 0;
	for (int i = 0; i < NUM_CHARS; ++i)
	{
		if (a.chars[i] != b.chars[i])
			dist++;
	}
	return dist;
}

void part1(const vector<Row> &rows)
{
	int twos = 0;
	int threes = 0;

	for (size_t i = 0; i < rows.size(); ++i)
	{
		twos += count_occurrences(rows[i], 2);
		threes += count_occurrences(rows[i], 3);
	}

	int result = twos * threes;
	cout << "Result: " << twos << " * " << threes << " = " << result << endl;
}

void part2(const vector<Row>& rows)
{
	for (size_t i = 0; i < rows.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < rows.size(); ++j)
		{
			int dist = hamming_distance(rows[i], rows[j]);
			if (dist == 1)
			{
				char result[NUM_CHARS - 1];
				int pos = 0;
				Row a = rows[i];
				Row b = rows[j];
				for (int k = 0; k < NUM_CHARS; ++k)
				{
					if (a.chars[k] == b.chars[k])
						result[pos++] = a.chars[k];
				}
				cout << "Result: " << (char*)result << endl;
			}
		}
	}
}

int main()
{
	ifstream file("input.txt");
	if (!file.is_open())
	{
		return EXIT_FAILURE;
	}

	vector<Row> rows;
	while (!file.eof())
	{
		string s;
		getline(file, s);

		if (file)
		{
			Row row;
			memcpy(&row, s.c_str(), sizeof(Row));
			rows.push_back(row);
		}
	}

	part1(rows);
	part2(rows);

	return EXIT_SUCCESS;
}
