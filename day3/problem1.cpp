#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cassert>
#include <set>

using namespace std;

int char_to_int(char c)
{
    int ret = static_cast<int>(c) - 'a' + 1;
    return ret < 0 ? ret + 31 + 27 : ret; // 'A' - 'a' + 1 = -31
}

int main()
{
    // open file
    ifstream file("./day3-input.txt");

    // go through each line
    string bag;
    int total = 0;
    while (file >> bag)
    {
        // O(n) solution
        size_t half_size = bag.size() / 2;
        char first_bag[half_size];
        char second_bag[half_size];

        // O(n) loop
        for (size_t i = 0; i < bag.size(); ++i)
        {
            if (i < half_size)
            {
                first_bag[i] = bag[i];
            }
            else
            {
                second_bag[i - half_size] = bag[i];
            }
        }

        // Sorting
        sort(first_bag, first_bag + half_size);
        sort(second_bag, second_bag + half_size);

        // O(n) intersection
        vector<char> intersection_result(half_size);
        vector<char>::iterator it = set_intersection(first_bag, first_bag + half_size, second_bag, second_bag + half_size, intersection_result.begin());

        set<char> used;
        // O(n) iteration
        for (auto i = intersection_result.begin(); i != it; ++i)
        {
            if (used.find(*i) == used.end())
            {
                used.insert(*i);
                total += char_to_int(*i);
                cout << *i << " : " << char_to_int(*i) << endl;
            }
        }
    }

    cout << "total was " << total << endl;

    return 0;
}