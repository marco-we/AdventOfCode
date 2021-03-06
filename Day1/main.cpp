#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

int main()
{
	vector<int> input;
	input.reserve(2000);

	ifstream file("input.txt");
	if (!file.is_open())
	{
		return EXIT_FAILURE;
	}

	int val = 0;
	while (!file.eof())
	{
		file >> val;
		input.push_back(val);
		val = 0;
	}

	int result = 0;
	set<int> frequencies;
	int iteration = 0;
	while (true)
	{
		for (size_t i = 0; i < input.size(); ++i)
		{
			frequencies.insert(result);
			result += input[i];

			if (frequencies.find(result) != frequencies.end())
			{
				cout << "First repeater is " << result << " after " << iteration << " iterations." << endl;
				return EXIT_SUCCESS;
			}
		}
		iteration++;
	}

	cout << "Result: " << result << endl;


	return EXIT_SUCCESS;
}
