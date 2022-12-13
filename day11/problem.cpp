#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <math.h>

using namespace std;

class Monkey
{
private:
    vector<uint64_t> items;
    function<uint64_t(uint64_t)> operation;
    uint64_t divisor;
    Monkey *give_to_true;
    Monkey *give_to_false;
    size_t num_inspected;

public:
    Monkey(vector<uint64_t> _items, function<uint64_t(uint64_t)> _operation) : items(_items), operation(_operation),
                                                                               give_to_false(nullptr), give_to_true(nullptr), num_inspected(0){};
    Monkey(){};

    void set_items(vector<uint64_t> &_items) { items = _items; }
    void set_operation(function<uint64_t(uint64_t)> op) { operation = op; }
    void set_divisor(uint64_t div) { divisor = div; }
    void set_give_to_true(Monkey *m) { give_to_true = m; }
    void set_give_to_false(Monkey *m) { give_to_false = m; }

    void inspect()
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            items[i] = operation(items[i]);
            ++num_inspected;
        }
    }

    void decrease_worry_level(uint64_t amt)
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            uint64_t times = items[i] / divisor;
            items[i] -= divisor * times;
        }
    }

    void test_items()
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            if (items[i] % divisor == 0)
            {
                give_to_true->items.push_back(items[i]);
            }
            else
            {
                give_to_false->items.push_back(items[i]);
            }
        }
        items.clear();
    }

    size_t get_num_inspected() { return num_inspected; }

    uint64_t get_divisor() { return divisor; }
};

void read_monkey(ifstream &file, vector<Monkey *> &monkeys)
{
    string reader;
    string reader_2;
    stringstream s;
    int num;
    int monkey_num;
    vector<uint64_t> numbers;
    function<uint64_t(uint64_t)> op;

    // get rid of the "Monkey x" line
    getline(file, reader);
    s << reader;
    s >> reader;
    s >> monkey_num;
    s >> reader;
    s.clear();
    cout << "monkey number: " << monkey_num << endl;

    // take care of starting items
    getline(file, reader);
    s << reader;
    for (size_t i = 0; i < 2; ++i)
    {
        s >> reader;
    }
    while (s)
    {
        s >> num;
        s >> reader;
        numbers.push_back(num);
        cout << num << ", ";
    }
    cout << endl;
    s.clear();
    monkeys[monkey_num]->set_items(numbers);

    // take care of operation
    getline(file, reader);
    s << reader;
    for (size_t i = 0; i < 5; ++i)
    {
        s >> reader;
    }
    s >> reader_2;
    if (reader_2[0] != 'o')
    {
        num = stol(reader_2);
        if (reader == "+")
        {
            op = [num](int x)
            { return x + num; };
        }
        else if (reader == "*")
        {
            op = [num](int x)
            { return x * num; };
        }
        cout << "operation is num = old " << reader << num << endl;
    }
    else
    {
        if (reader == "+")
        {
            op = [](int x)
            {
                return x + x;
            };
        }
        else if (reader == "*")
        {
            op = [](int x)
            { return x * x; };
        }
        cout << "operation is num = old " << reader << "old" << endl;
    }
    s.clear();
    monkeys[monkey_num]->set_operation(op);

    // take care of divisible by
    getline(file, reader);
    s << reader;
    for (size_t i = 0; i < 3; ++i)
    {
        s >> reader;
    }
    s >> num;
    s.clear();
    cout << "testing divisible by " << num << endl;
    monkeys[monkey_num]->set_divisor(num);

    // take care of the monkeys
    for (size_t i = 0; i < 2; ++i)
    {
        getline(file, reader);
        s << reader;
        for (size_t x = 0; x < 5; ++x)
        {
            s >> reader;
        }
        s >> num;
        if (i == 0)
        {
            monkeys[monkey_num]->set_give_to_true(monkeys[num]);
        }
        else
        {
            monkeys[monkey_num]->set_give_to_false(monkeys[num]);
        }
        s.clear();
    }

    // get last line
    getline(file, reader);
}
vector<Monkey *> read_monkeys(string file_name)
{
    ifstream file(file_name);
    size_t max_len = 100;
    vector<Monkey *> monkeys;
    for (size_t i = 0; i < max_len; ++i)
    {
        monkeys.push_back(new Monkey());
    }

    size_t i = 0;
    while (file)
    {
        read_monkey(file, monkeys);
        ++i;
    }
    for (size_t x = i; x < max_len; ++x)
    {
        delete monkeys.back();
        monkeys.pop_back();
    }
    return monkeys;
}

bool is_prime(uint64_t x)
{
    for (uint64_t i = 2; i <= static_cast<uint64_t>(sqrt(x)); ++i)
    {
        if (x % i == 0)
        {
            return false;
        }
    }
    return true;
}

uint64_t factorial_primes(uint64_t x)
{
    uint64_t ret = 1;
    for (uint64_t i = x; i > 1; --i)
    {
        if (is_prime(i))
        {
            ret *= i;
            cout << "i is " << i << endl;
        }
    }
    return ret;
}

int main()
{
    vector<Monkey *> monkeys = read_monkeys("day11-input.txt");
    cout << "monkeys size is" << monkeys.size() << endl;

    uint64_t max_divisor = 1;
    for (auto &monkey : monkeys)
    {
        max_divisor = max(max_divisor, monkey->get_divisor());
    }
    max_divisor = factorial_primes(23);
    cout << "max divisor is " << max_divisor << endl;

    size_t rounds = 20;
    for (size_t i = 0; i < rounds; ++i)
    {
        for (auto &monkey : monkeys)
        {
            monkey->inspect();
            monkey->decrease_worry_level(max_divisor);
            monkey->test_items();
        }
    }

    vector<size_t> monkey_business;
    for (auto &monkey : monkeys)
    {
        cout << "monkey inspected " << monkey->get_num_inspected() << " times" << endl;
        monkey_business.push_back(monkey->get_num_inspected());
    }
    sort(monkey_business.begin(), monkey_business.end());
    cout << "total monkey business was " << monkey_business[monkey_business.size() - 1] * monkey_business[monkey_business.size() - 2] << endl;

    // cleanup
    for (size_t i = 0; i < monkeys.size(); ++i)
    {
        delete monkeys[i];
        monkeys[i] = nullptr;
    }

    return 0;
}