#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int main()
{
    // open file
    ifstream file("./problem1-input.txt");

    int max_calories = 0;

    // read each number
    string temp;
    int total = 0;
    int reader = 0;
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
            max_calories = max(max_calories, total);
            total = 0;
            reader = 0;
        }
    }

    cout << "final answer" << endl;
    cout << max_calories << endl;
}