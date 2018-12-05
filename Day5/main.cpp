#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

int main(int argc, char** argv)
{
	ifstream file("test.txt", std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		return EXIT_FAILURE;
	}
	
	unsigned int filesize = (unsigned int)file.tellg() - 1;
	unsigned int orgsize = filesize;
	file.seekg(0, std::ios::beg);

	int8_t* read_buffer = new int8_t[filesize];
	int8_t* write_buffer = new int8_t[filesize];
	if (!file.read((char*)read_buffer, filesize))
	{
		return EXIT_FAILURE;
	}

	
	
	bool changed = false;
	do
	{
		changed = false;
		unsigned int write_pos = 0;
		memset(write_buffer, 0, orgsize);
		for (unsigned int i = 0; i < filesize - 1;)
		{
			int8_t a = read_buffer[i];
			int8_t b = read_buffer[i + 1];

			int8_t diff = std::abs(a - b);
			if (diff == 32)
			{
				i += 2;
				changed = true;
			}
			else
			{
				write_buffer[write_pos++] = a;
				i++;
			}
		}
		
		std::swap(read_buffer, write_buffer);
		filesize = write_pos;

	} while (changed && filesize > 0);

	ofstream debug("output.txt");
	if (debug.is_open())
	{
		debug.write((char*)read_buffer, filesize);
	}


	delete[] read_buffer;
	delete[] write_buffer;
	return EXIT_SUCCESS;
}
