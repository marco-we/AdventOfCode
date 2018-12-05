#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
using namespace std;

#if 0
struct Claim
{
    Claim(int _id, int _x, int _y, int _w, int _h) :
        id(_id), x(_x), y(_y), w(_w), h(_h) {}

    Claim(int _x, int _y, int _w, int _h) :
        id(-1), x(_x), y(_y), w(_w), h(_h) {}

    int id;
    int16_t x, y;
    int16_t w, h;

    int16_t x_max() const { return x + w - 1; }
    int16_t y_max() const { return y + h - 1; }
};
#else
struct Claim
{
    Claim(int _id, int _x, int _y, int _w, int _h) :
        id(_id), x(_x), y(_y), w(_w), h(_h)
    {
        max_x = x_max();
        max_y = y_max();
    }

    Claim(int _x, int _y, int _w, int _h) :
        id(-1), x(_x), y(_y), w(_w), h(_h)
    {
        max_x = x_max();
        max_y = y_max();
    }

    int id;
    int16_t x, y;
    int16_t w, h;
    int16_t max_x, max_y;

    int16_t x_max() const { return x + w - 1; }
    int16_t y_max() const { return y + h - 1; }
};

#endif

bool are_disjoint(Claim a, Claim b)
{
    const bool disjoint_hor = a.x > b.x_max() || b.x > a.x_max();
    const bool disjoint_ver = a.y > b.y_max() || b.y > a.y_max();
    return disjoint_hor || disjoint_ver;
}

const int MAX_LENGTH = 1000;
int16_t fabric[MAX_LENGTH][MAX_LENGTH];

void part1(const vector<Claim>& claims)
{
    memset(fabric, 0, sizeof(int16_t) * 1000 * 1000);

    for (size_t i = 0; i < claims.size(); ++i)
    {
        Claim c = claims[i];
        for (int16_t y = 0; y < c.h; ++y)
        {
            for (int16_t x = 0; x < c.w; ++x)
            {
                fabric[c.y + y][c.x + x]++;
            }
        }
    }

    int counter = 0;
    for (int y = 0; y < MAX_LENGTH; ++y)
    {
        for (int x = 0; x < MAX_LENGTH; ++x)
        {
            if (fabric[y][x] > 1)
                counter++;
        }
    }
}

void part2(const vector<Claim>& claims)
{
    const size_t num_claims = claims.size();
    for (size_t i = 0; i < num_claims; ++i)
    {
        const Claim c = claims[i];

        bool found_overlap = false;
        for (size_t j = 0; j < num_claims; ++j)
        {
            if (j == i)
                continue;

            if (!are_disjoint(c, claims[j]))
            {
                found_overlap = true;
                break;
            }
        }

        if (!found_overlap)
        {
            cout << "Interval " << c.id << " does not overlap." << endl;
            break;
        }
    }
}

int main(int argc, char** argv)
{
    ifstream file("input.txt");
    if (!file.is_open())
    {
        return EXIT_FAILURE;
    }

    vector<Claim> claims;
    while (!file.eof())
    {
        string s;
        getline(file, s);

        if (file)
        {
            int id, x, y, w, h;
            if (sscanf(s.c_str(), "#%d @ %d,%d: %dx%d", &id, &x, &y, &w, &h) != 5)
                return EXIT_FAILURE;

            claims.push_back(Claim(id, x, y, w, h));
        }
    }

    //part1(claims);
    part2(claims);

    return EXIT_SUCCESS;
}