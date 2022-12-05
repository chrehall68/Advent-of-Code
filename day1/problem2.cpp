#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int main()
{
    // open file
    ifstream file("./day1-input.txt");

    int max_calories = 0;

    // read each number
    string temp;
    int total = 0;
    int reader = 0;
    vector<int> elves;
    while (getline(file, temp))
    {
        if (!temp.empty())
        {
            reader = stol(temp);
            cout << reader << endl;
            total += reader;
        }
        else
        {
            elves.push_back(total);
            total = 0;
            reader = 0;
        }
    }

    sort(elves.begin(), elves.end());
    size_t size = elves.size();
    cout << "max, 2nd max, 3rd max: " << elves[size - 1] << ", " << elves[size - 2] << ", " << elves[size - 3] << endl;
    cout << "sum is " << elves[size - 1] + elves[size - 2] + elves[size - 3] << endl;

    return 0;
}