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
    map<char, char> other_move_map = {{'A', 'X'}, {'B', 'Y'}, {'C', 'Z'}};
    map<char, int> move_val = {{'X', 1}, {'Y', 2}, {'Z', 3}};

    string temp;
    int total = 0;
    while (getline(file, temp))
    {
        char other_move = other_move_map[temp[0]];
        char your_move = temp[2];

        int score = move_val[your_move];

        // check cases of whether you won
        if (your_move == other_move)
        {
            // draw
            score += 3;
        }
        else if (
            (your_move == 'X' && other_move == 'Z')    // rock vs scissors
            || (your_move == 'Y' && other_move == 'X') // paper vs rock
            || (your_move == 'Z' && other_move == 'Y') // scissors vs paper
        )
        {
            // win
            score += 6;
        }
        total += score;
    }

    cout << "score is " << total << endl;
}