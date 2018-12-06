#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

int main(int argc, char** argv)
{
	ifstream file("input.txt", std::ios::ate);
	if (!file.is_open())
	{
		return EXIT_FAILURE;
	}

	unsigned int filesize = (unsigned int)file.tellg();
	unsigned int orgsize = filesize;
	file.seekg(0, std::ios::beg);

    int8_t* org_buffer = new int8_t[filesize];
	int8_t* read_buffer = new int8_t[filesize];
	int8_t* write_buffer = new int8_t[filesize];
	if (!file.read((char*)org_buffer, filesize))
	{
		return EXIT_FAILURE;
	}

    unsigned int min_size = UINT_MAX;
    int8_t best_char = -1;
    for (int8_t n = 65; n < 91; ++n)
    {
        filesize = orgsize;
        memcpy(read_buffer, org_buffer, orgsize);

        const int8_t lower_n = n + 32;
        do
        {
            unsigned int write_pos = 0;
            memset(write_buffer, 0, orgsize);
            for (unsigned int i = 0; i < filesize;)
            {
                int8_t a = read_buffer[i];
                int8_t b = read_buffer[i + 1];

                if (a == n || a == lower_n)
                {
                    ++i;
                    continue;
                }

                if (i == filesize - 1)
                {
                    write_buffer[write_pos++] = a;
                    break;
                }

                int8_t diff = std::abs(a - b);
                if (diff == 32)
                {
                    i += 2;
                }
                else
                {
                    write_buffer[write_pos++] = a;
                    i++;
                }
            }

            std::swap(read_buffer, write_buffer);

            if (filesize == write_pos)
                break;

            filesize = write_pos;

        } while (filesize > 0);

        if (filesize < min_size)
        {
            min_size = filesize;
            best_char = n;
        }
    }

#if 0
	ofstream debug("output.txt");
	if (debug.is_open())
	{
		debug.write((char*)read_buffer, filesize);
	}
#endif

	delete[] read_buffer;
	delete[] write_buffer;
	return EXIT_SUCCESS;
}
