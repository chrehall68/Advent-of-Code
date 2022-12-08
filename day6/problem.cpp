#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <deque>
#include <set>

using namespace std;

template <typename T>
class OverflowingStack
{
private:
    size_t max_size;
    size_t cur_size;
    T *buffer;

    void rotate()
    {
        for (size_t i = 0; i < max_size - 1; ++i)
        {
            buffer[i] = buffer[i + 1];
        }
    }

public:
    OverflowingStack(size_t _max_size) : max_size(_max_size), cur_size(0), buffer(new T[max_size]){};
    ~OverflowingStack() { delete[] buffer; }
    void push(T val)
    {
        if (cur_size < max_size)
        {
            buffer[cur_size] = val;
            ++cur_size;
        }
        else
        {
            rotate();
            buffer[max_size - 1] = val;
        }
    }

    /**
     * Returns copies of the items within the buffer
     */
    vector<T> get_items() const
    {
        vector<T> ret(max_size);
        for (size_t i = 0; i < max_size; ++i)
        {
            ret.push_back(buffer[i]);
        }
        return ret;
    }

    /**
     * O(n) implementation
     */
    bool all_equals() const
    {
        if (cur_size != max_size)
        {
            return false;
        }

        for (size_t i = 0; i < max_size - 1; ++i)
        {
            if (buffer[i] != buffer[i + 1])
            {
                return false;
            }
        }
        return true;
    }

    /**
     * O(NlogN) implementation
     */
    bool all_different() const
    {
        if (cur_size != max_size)
        {
            return false;
        }

        set<T> temp;
        for (size_t i = 0; i < max_size; ++i)
        {                            // O(N) loop
            temp.emplace(buffer[i]); // insertion is O(logN)
        }
        return temp.size() == max_size;
    }
};

int main()
{
    // open file
    ifstream file("./day6-input.txt");
    OverflowingStack<int> stack(14);

    // go through each
    char temp;
    size_t current = 0;
    while (file)
    {
        ++current;
        file.read(&temp, 1);
        stack.push(static_cast<int>(temp));
        if (stack.all_different())
        {
            break;
        }
    }

    cout << "finished at position " << current << endl;
    return 0;
}