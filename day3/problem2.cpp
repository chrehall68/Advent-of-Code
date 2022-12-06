#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <string>

using namespace std;

int char_to_int(char c)
{
    int ret = c - 'a' + 1;
    return ret < 0 ? ret + 31 + 27 : ret;
}
int main()
{
    // open file
    ifstream file("./day3-input.txt");

    // go through each group of 3 lines
    int total = 0;
    while (1)
    {
        vector<char> bags[3];
        string temp;
        size_t size = 0;

        for (size_t i = 0; i < 3; ++i)
        {
            if (!(file >> temp))
            {
                break; // save operations
            }

            // O(n)
            for (int x = 0; x < temp.size(); ++x)
            {
                bags[i].push_back(temp[x]);
            }
            size = max(size, temp.size());
            sort(bags[i].begin(), bags[i].end());
        }
        if (!file)
        {
            break;
        }

        // first intersection
        vector<char> intersection_result_1(size);
        vector<char>::iterator it = set_intersection(bags[0].begin(), bags[0].end(), bags[1].begin(), bags[1].end(), intersection_result_1.begin());

        // second intersection
        vector<char> intersection_final_result(1);
        set_intersection(intersection_result_1.begin(), it, bags[2].begin(), bags[2].end(), intersection_final_result.begin());

        // add to total
        total += char_to_int(intersection_final_result[0]);
    }

    cout << "total was " << total << endl;

    return 0;
}