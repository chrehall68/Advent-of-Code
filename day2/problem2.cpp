#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

int main()
{
    // open file
    ifstream file("./day2-input.txt");

    // rock, paper, scissors
    // 1      2       3
    map<char, map<char, int>> val_map = {
        {'A', {{'X', 3}, {'Y', 1}, {'Z', 2}}}, // rock
        {'B', {{'X', 1}, {'Y', 2}, {'Z', 3}}}, // paper
        {'C', {{'X', 2}, {'Y', 3}, {'Z', 1}}}  // scissors
    };
    map<char, int> move_val = {{'X', 0}, {'Y', 3}, {'Z', 6}};

    string temp;
    int total = 0;
    while (getline(file, temp))
    {
        char other_move = temp[0];
        char your_move = temp[2];

        total += move_val[your_move];
        total += val_map[other_move][your_move];
    }

    cout << "score is " << total << endl;
}