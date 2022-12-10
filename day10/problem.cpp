#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class Cycler
{
private:
    int value;
    size_t cur_cycle;

    size_t last_cycle;
    int to_add_value;
    string op;

public:
    Cycler() : value(1), cur_cycle(0), last_cycle(0), to_add_value(0){};

    void addx(int value)
    {
        to_add_value = value;
        last_cycle = cur_cycle;
        op = "addx";
    }
    void noop()
    {
        op = "noop";
    }

    bool update()
    {
        ++cur_cycle;
        if (op == "addx" && last_cycle + 2 == cur_cycle)
        {
            value += to_add_value;
            op = "";
            return true;
        }
        if (op == "noop")
        {
            op = "";
            return true;
        }
        return false;
    }

    size_t get_cycle()
    {
        return cur_cycle;
    }
    int get_amt()
    {
        return value;
    }
};

int main()
{
    ifstream file("./day10-input.txt");

    // part 1 vars
    Cycler c;
    string reader;
    stringstream s;
    int amt;
    vector<int> times = {20, 60, 100, 140, 180, 220};

    // part 2
    char output[6][40];

    int total = 0;
    while (getline(file, reader))
    {
        s << reader;
        s >> reader;

        if (reader == "noop")
        {
            c.noop();
        }
        else
        {
            s >> amt;
            c.addx(amt);
        }
        do
        {
            int c_cycle = c.get_cycle() + 1;
            // part 1
            if (find(times.begin(), times.end(), c_cycle) != times.end())
            {
                total += c.get_amt() * c_cycle;
            }
            // part 2
            c_cycle = c.get_cycle();
            if (abs(c_cycle % 40 - c.get_amt() % 40) <= 1)
            {
                output[c_cycle / 40][c_cycle % 40] = '#';
            }
            else
            {
                output[c_cycle / 40][c_cycle % 40] = '.';
            }

        } while (!c.update());
        s.clear();
    }

    cout << "finished with " << total << " at cycle " << c.get_cycle() << endl;
    for (size_t i = 0; i < 6; ++i)
    {
        for (size_t x = 0; x < 40; ++x)
        {
            cout << output[i][x];
        }
        cout << endl;
    }
}