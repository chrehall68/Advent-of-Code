#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>

using namespace std;

size_t get_best_view_score(vector<vector<uint8_t>> &map)
{
    size_t best = 0;
    size_t width = map[0].size();
    size_t height = map.size();
    cout << "width is " << width << endl;
    cout << "height is " << height << endl;
    for (size_t row = 1; row < height - 1; ++row)
    {
        for (size_t col = 1; col < width - 1; ++col)
        {
            // left
            size_t left_score = 1;
            for (size_t cur_col = col - 1; cur_col > 0; --cur_col, ++left_score)
            {
                if (map[row][cur_col] >= map[row][col])
                {
                    break;
                }
            }

            // right
            size_t right_score = 1;
            for (size_t cur_col = col + 1; cur_col < width - 1; ++cur_col, ++right_score)
            {
                if (map[row][cur_col] >= map[row][col])
                {
                    break;
                }
            }

            // up
            size_t up_score = 1;
            for (int cur_row = row - 1; cur_row > 0; --cur_row, ++up_score)
            {
                if (map[cur_row][col] >= map[row][col])
                {
                    break;
                }
            }

            // down
            size_t down_score = 1;
            for (size_t cur_row = row + 1; cur_row < height - 1; ++cur_row, ++down_score)
            {
                if (map[cur_row][col] >= map[row][col])
                {
                    break;
                }
            }

            size_t score = left_score * right_score * up_score * down_score;
            best = max(score, best);
        }
    }
    return best;
}

vector<vector<uint8_t>> set_visible(vector<vector<uint8_t>> &map)
{
    // initialize the visual map
    size_t width = map[0].size();
    size_t height = map.size();
    vector<vector<uint8_t>> ret(height);
    for (size_t i = 0; i < height; ++i)
    {
        vector<uint8_t> row(width);
        ret[i] = row;
    }

    // set edges to 1
    for (size_t i = 0; i < width; ++i)
    {
        ret[0][i] = 1;
        ret[height - 1][i] = 1;
    }
    for (size_t i = 0; i < height; ++i)
    {
        ret[i][0] = 1;
        ret[i][width - 1] = 1;
    }

    // go through the columns
    for (size_t row = 1; row < height - 1; ++row)
    {
        // left traversal
        uint8_t row_max = map[row][0];
        for (size_t col = 1; col < width - 1; ++col)
        {
            if (map[row][col] > row_max)
            {
                ret[row][col] = 1;
                row_max = map[row][col];
            }
        }

        // right traversal
        row_max = map[row][width - 1];
        for (size_t col = width - 2; col > 0; --col)
        {
            if (map[row][col] > row_max)
            {
                ret[row][col] = 1;
                row_max = map[row][col];
            }
        }
    }

    // go through the rows
    for (size_t col = 1; col < width - 1; ++col)
    {
        // left traversal
        uint8_t col_max = map[0][col];
        for (size_t row = 1; row < height - 1; ++row)
        {
            if (map[row][col] > col_max)
            {
                ret[row][col] = 1;
                col_max = map[row][col];
            }
        }

        // right traversal
        col_max = map[height - 1][col];
        for (size_t row = height - 2; row > 0; --row)
        {
            if (map[row][col] > col_max)
            {
                ret[row][col] = 1;
                col_max = map[row][col];
            }
        }
    }

    return ret;
}

int main()
{
    // open file
    ifstream file("./day8-input.txt");

    // init vars
    vector<vector<uint8_t>> map;
    vector<uint8_t> current;
    char reader;

    // read file
    while (file.get(reader))
    {
        if (reader == '\n')
        {
            map.push_back(current);
            current.clear();
        }
        else
        {
            current.push_back(reader - '0');
        }
    }
    if (current.size() != 0)
    {
        map.push_back(current);
    }

    vector<vector<uint8_t>> result = set_visible(map);
    size_t total = 0;
    for (auto &row : result)
    {
        for (auto &col : row)
        {
            if (col == 1)
            {
                ++total;
            }
        }
    }
    cout << "total was " << total << endl;

    size_t best_score = get_best_view_score(map);
    cout << "best view score was " << best_score << endl;

    return 0;
}