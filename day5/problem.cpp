#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <sstream>
#include <cassert>

using namespace std;

class ShipCargo
{
private:
    vector<stack<char>> stacks;

public:
    friend istream &operator>>(istream &file, ShipCargo &cargo)
    {
        // init vars
        vector<string> lines;
        string line;

        // read all the lines
        size_t i = 0;
        while (1)
        {
            getline(file, line);
            lines.push_back(line);
            if (lines[i].find('1') != string::npos)
            {
                break; // has a number, so is the end of the cargo
            }
            ++i;
        }

        // determine the number of stacks
        istringstream o(lines[i]);
        size_t num_stacks;
        while (o >> num_stacks)
            ;
        cout << "num stacks is " << num_stacks << endl;

        // init the stacks
        for (size_t i = 0; i < num_stacks; ++i)
        {
            cargo.stacks.push_back(stack<char>());
        }

        // add the items
        for (int row = lines.size() - 2; row > -1; --row)
        {
            for (int stack = 0; stack < num_stacks; ++stack)
            {
                char current = lines[row][1 + stack * 4]; // hardcoded position
                if (current != ' ')
                {
                    cargo.stacks[stack].push(current);
                }
            }
        }

        return file;
    }

    void move(size_t amt, size_t from, size_t to)
    {
        from = from - 1;
        to = to - 1;
        for (size_t i = 0; i < amt; ++i)
        {
            stacks[to].push(stacks[from].top());
            stacks[from].pop();
        }
    }

    void move_part2(size_t amt, size_t from, size_t to)
    {
        // adjust to computer values
        from = from - 1;
        to = to - 1;

        // first, put it in a placeholder
        stack<char> temp;
        for (size_t i = 0; i < amt; ++i)
        {
            temp.push(stacks[from].top());
            stacks[from].pop();
        }

        // then, put it in the result
        for (size_t i = 0; i < amt; ++i)
        {
            stacks[to].push(temp.top());
            temp.pop();
        }
    }

    vector<char> tops()
    {
        vector<char> ret;
        for (auto stack : stacks)
        {
            ret.push_back(stack.top());
        }
        return ret;
    }
};

int main()
{
    ifstream file("./day5-input.txt");
    ShipCargo cargo;
    file >> cargo;

    string temp;
    size_t amt, from, to;
    while (1)
    {
        file >> temp >> amt >> temp >> from >> temp >> to;
        if (!file)
        {
            break;
        }

        cargo.move_part2(amt, from, to);

        amt = 0, from = 0, to = 0;
    }

    for (char c : cargo.tops())
    {
        cout << c;
    }
    cout << endl;

    // cleanup
    file.close();
    return 0;
}