#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

pair<string, string> split(string inp, char separator)
{
    size_t separator_pos = inp.find(separator);
    pair<string, string> ret = {inp.substr(0, separator_pos),
                                inp.substr(separator_pos + 1)};
    return ret;
}

class ReaderIterator
{
private:
    ifstream file;
    string line;
    bool good;

public:
    ReaderIterator(string file_name) : file(file_name) { good = static_cast<bool>(getline(file, line)); };
    ReaderIterator &operator++()
    {
        good = static_cast<bool>(getline(file, line));
        return *this;
    }

    operator bool()
    {
        return good;
    }

    pair<pair<int, int>, pair<int, int>> process()
    {
        // separate out
        pair<string, string> halves = split(line, ',');
        pair<string, string> str_numbers_1 = split(halves.first, '-');
        pair<string, string> str_numbers_2 = split(halves.second, '-');

        // turn into ints
        pair<int, int> numbers_1 = {stol(str_numbers_1.first), stol(str_numbers_1.second)};
        pair<int, int> numbers_2 = {stol(str_numbers_2.first), stol(str_numbers_2.second)};
        return {numbers_1, numbers_2};
    }

    void close()
    {
        file.close();
    }
};

int part1()
{
    ReaderIterator it("./day4-input.txt");
    int total = 0;

    while (it)
    {
        auto numbers = it.process();
        auto numbers_1 = numbers.first, numbers_2 = numbers.second;

        // compare
        if ((numbers_1.first <= numbers_2.first && numbers_1.second >= numbers_2.second) ||
            (numbers_2.first <= numbers_1.first && numbers_2.second >= numbers_1.second))
        {
            ++total;
        }

        ++it;
    }

    it.close();
    return total;
}

int part2()
{
    ReaderIterator it("./day4-input.txt");
    int total = 0;
    while (it)
    {
        auto numbers = it.process();
        auto numbers_1 = numbers.first, numbers_2 = numbers.second;

        // compare
        if ((numbers_1.first >= numbers_2.first && numbers_1.first <= numbers_2.second) ||
            (numbers_1.second >= numbers_2.first && numbers_1.second <= numbers_2.second) ||
            (numbers_2.first >= numbers_1.first && numbers_2.first <= numbers_1.second) ||
            (numbers_2.second >= numbers_1.first && numbers_2.second <= numbers_1.second))
        {
            ++total;
        }
        ++it;
    }
    it.close();
    return total;
}

int main()
{
    cout << "part 1 : " << part1() << endl;
    cout << "part 1 : " << part2() << endl;
    return 0;
}