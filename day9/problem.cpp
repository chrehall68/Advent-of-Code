#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

class Board
{
private:
    struct BoardSpace
    {
        char name;
        bool visited_by_tail;

        BoardSpace() : name('.'), visited_by_tail(false){};
        friend ostream &operator<<(ostream &o, BoardSpace &space)
        {
            char rep = '.';
            if (space.visited_by_tail)
            {
                rep = 'X';
            }
            if (space.name != '.')
            {
                rep = space.name;
            }
            return o << rep;
        }
        void clear_name()
        {
            name = '.';
        }
    };

    struct Knot
    {
        char name;
        int x;
        int y;
        Knot *next;
        bool is_tail;

        Knot(char _name, Knot *_next, bool _is_tail = false) : name(_name),
                                                               next(_next),
                                                               x(0), y(0),
                                                               is_tail(_is_tail){};
        ~Knot()
        {
            next = nullptr;
        }
        void follow(vector<vector<BoardSpace>> &board)
        {
            if (abs(max(abs(x - next->x), 1) * max(abs(y - next->y), 1) >= 2))
            {
                board[y][x].clear_name();

                // diagonal to up/down or left/right
                if ((abs(next->y - y) >= 2 && abs(next->x - x) >= 1) || (abs(next->x - x) >= 2 && abs(next->y - y) >= 1))
                {
                    if (next->y > y)
                    {
                        ++y;
                    }
                    else
                    {
                        --y;
                    }
                    if (next->x > x)
                    {
                        ++x;
                        ;
                    }
                    else
                    {
                        --x;
                    }
                }
                // vertical displacement
                else if (abs(next->y - y) >= 2)
                {
                    // head below --> tail move down
                    if (next->y > y)
                    {
                        ++y;
                    }
                    else
                    {
                        // head above --> tail move up
                        --y;
                    }
                }
                // horizontal displacement
                else if (abs(next->x - x) >= 2)
                {
                    // head right --> tail move right
                    if (next->x > x)
                    {
                        ++x;
                        ;
                    }
                    else
                    {
                        // head left --> tail move left
                        --x;
                    }
                }

                if (is_tail)
                {
                    board[y][x].visited_by_tail = true;
                }
            }
            if (board[y][x].name == '.')
            {
                board[y][x].name = name;
            }
        }
    };
    // board attrs
    vector<vector<BoardSpace>> board;
    size_t width;
    size_t height;
    vector<Knot *> knots;
    Knot *head;

    /**
     * add a row on the top of the board
     */
    void add_row_top()
    {
        ++height;
        vector<BoardSpace> row(width);
        board.emplace(board.begin(), row);

        // increase it because we added a row above
        for (size_t j = 0; j < knots.size(); ++j)
        {
            ++(knots[j]->y);
        }
        ++head->y;
    }
    /**
     * add a row on the bottom of the board
     */
    void add_row_bottom()
    {
        ++height;
        vector<BoardSpace> row(width);
        board.push_back(row);
    }
    /**
     * add a column to the left of the board
     */
    void add_col_left()
    {
        ++width;
        for (auto &row : board)
        {
            row.emplace(row.begin(), BoardSpace());
        }

        // increase it because we added a col on the left
        for (size_t j = 0; j < knots.size(); ++j)
        {
            ++(knots[j]->x);
        }
        ++head->x;
    }
    /**
     * add a column to the right of the board
     */
    void add_col_right()
    {
        ++width;
        for (auto &row : board)
        {
            row.push_back(BoardSpace());
        }
    }

public:
    Board(size_t num_tails = 1) : width(0), height(0), head(new Knot('H', nullptr))
    {
        add_row_bottom();
        add_col_right();

        // set the node to the head (and the tail)
        board[0][0].visited_by_tail = true;
        Knot *prev = head;
        for (size_t i = 0; i < num_tails; ++i)
        {
            char name = i == num_tails - 1 ? 'T' : '1' + i;
            Knot *next = new Knot(name, prev, i == num_tails - 1);
            knots.push_back(next);
            prev = next;
        }
    }

    void move_left(size_t amt = 1)
    {
        for (size_t i = 0; i < amt; ++i)
        {
            if (head->x == 0)
            {
                add_col_left();
            }
            board[head->y][head->x].clear_name();
            --head->x;
            board[head->y][head->x].name = head->name;

            for (size_t j = 0; j < knots.size(); ++j)
            {
                knots[j]->follow(board);
            }
        }
    }
    void move_right(size_t amt)
    {
        for (size_t i = 0; i < amt; ++i)
        {
            if (head->x == width - 1)
            {
                add_col_right();
            }
            board[head->y][head->x].clear_name();
            ++head->x;
            board[head->y][head->x].name = head->name;

            for (size_t j = 0; j < knots.size(); ++j)
            {
                knots[j]->follow(board);
            }
        }
    }
    void move_up(size_t amt)
    {
        for (size_t i = 0; i < amt; ++i)
        {
            if (head->y == 0)
            {
                add_row_top();
            }
            board[head->y][head->x].clear_name();
            --head->y;
            board[head->y][head->x].name = head->name;

            for (size_t j = 0; j < knots.size(); ++j)
            {
                knots[j]->follow(board);
            }
        }
    }
    void move_down(size_t amt)
    {
        for (size_t i = 0; i < amt; ++i)
        {
            if (head->y == height - 1)
            {
                add_row_bottom();
            }
            board[head->y][head->x].clear_name();
            ++head->y;
            board[head->y][head->x].name = head->name;

            for (size_t j = 0; j < knots.size(); ++j)
            {
                knots[j]->follow(board);
            }
        }
    }

    size_t get_num_tail_visited()
    {
        size_t total = 0;
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                if (board[y][x].visited_by_tail)
                {
                    ++total;
                }
            }
        }
        return total;
    }

    friend ostream &operator<<(ostream &o, Board &b)
    {
        for (auto &row : b.board)
        {
            for (auto &col : row)
            {
                o << col << " ";
            }
            o << endl;
        }
        return o;
    }
};

int main()
{
    // open file
    ifstream file("./day9-input.txt");

    // init vars
    Board board(9);
    stringstream s;
    string reader;
    int amt = 0;

    // process file
    while (getline(file, reader))
    {
        s << reader;
        s >> reader;
        s >> amt;

        if (reader == "D")
        {
            board.move_down(amt);
        }
        else if (reader == "L")
        {
            board.move_left(amt);
        }
        else if (reader == "U")
        {
            board.move_up(amt);
        }
        else if (reader == "R")
        {
            board.move_right(amt);
        }
        s.clear();
        // cout << board << endl;
    }

    cout << "finished with " << board.get_num_tail_visited() << " tail-visited spaces" << endl;

    file.close();
    return 0;
}