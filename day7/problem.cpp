#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <limits>

using namespace std;

class ComputerObject
{
private:
    string name;
    uint64_t size;
    ComputerObject *parent;

public:
    ComputerObject(string _name, uint64_t _size) : name(_name), size(_size), parent(nullptr){};
    uint64_t get_size() const
    {
        return size;
    }
    string get_name() const
    {
        return name;
    }
    virtual string get_type() const = 0;

    ComputerObject *get_parent()
    {
        return parent;
    }

    void set_size(uint64_t _size)
    {
        size = _size;
    }
    void set_parent(ComputerObject *_parent)
    {
        parent = _parent;
    }
};

class File : public ComputerObject
{
public:
    File(string _name, uint64_t _size) : ComputerObject(_name, _size){};
    virtual string get_type() const
    {
        return "file";
    }
};

class Directory : public ComputerObject
{
private:
    vector<ComputerObject *> contents;

public:
    Directory(string _name) : ComputerObject(_name, 0), contents(){};
    ~Directory()
    {
        for (ComputerObject *content : contents)
        {
            delete content;
        }
    }
    void update()
    {
        // get correct size for this level
        uint64_t temp_size = 0;
        for (ComputerObject *content : contents)
        {
            temp_size += content->get_size();
        }
        set_size(temp_size);

        // recursively go up
        if (get_parent() != nullptr)
        {
            static_cast<Directory *>(get_parent())->update();
        }
    }
    void add_child(ComputerObject *child)
    {
        child->set_parent(this);
        contents.push_back(child);
        update();
    }
    virtual string get_type() const
    {
        return "directory";
    }

    vector<ComputerObject *> get_contents() const
    {
        return contents;
    }

    void traverse_child_directories(function<void(uint64_t)> func)
    {
        vector<Directory *> current;
        for (uint64_t i = 0; i < contents.size(); ++i)
        {
            if (contents[i]->get_type() == "directory")
            {
                func(contents[i]->get_size());
                current.push_back(static_cast<Directory *>(contents[i]));
            }
        }

        // bfs
        vector<Directory *> next;
        while (current.size() != 0)
        {
            for (Directory *dir : current)
            {
                for (uint64_t i = 0; i < dir->contents.size(); ++i)
                {
                    if (dir->contents[i]->get_type() == "directory")
                    {
                        func(dir->contents[i]->get_size());
                        next.push_back(static_cast<Directory *>(dir->contents[i]));
                    }
                }
            }

            current = next;
            next.clear();
        }
    }
};

int part1_condition(uint64_t amt, uint64_t limit)
{
    static int total = 0;
    if (amt < limit)
    {
        total += amt;
    }
    return total;
}
void part2_condition(uint64_t amt, uint64_t necessary, uint64_t *_min)
{
    *_min = (amt > necessary ? min<uint64_t>(amt, *_min) : *_min);
}

int main()
{
    // open file
    ifstream file("./day7-input.txt");

    // initialize vars
    Directory root("ROOT");
    root.add_child(new Directory("/"));
    string reader;
    stringstream line;
    Directory *current = &root;

    // go through the file
    while (getline(file, reader))
    {
        line << reader;

        // some sort of command
        if (reader[0] == '$')
        {
            // get rid of the $
            line >> reader;
            // check command, only do something if its cd
            line >> reader;
            if (reader == "cd")
            {
                line >> reader;

                // switch to parent
                if (reader == "..")
                {
                    current = static_cast<Directory *>(current->get_parent());
                }
                // switch to the right child
                for (ComputerObject *content : current->get_contents())
                {
                    if (content->get_type() == "directory" && content->get_name() == reader)
                    {
                        current = static_cast<Directory *>(content);
                        break;
                    }
                }
            }
        }

        // not a command, is listing out files / directories
        else
        {
            line >> reader;
            // directory
            if (reader[0] == 'd')
            {
                line >> reader;
                current->add_child(new Directory(reader));
            }
            // file
            else
            {
                int size = stol(reader);
                line >> reader;
                current->add_child(new File(reader, size));
            }
        }
        line.clear();
    }

    // part 1
    uint64_t limit = 100000;
    function<void(uint64_t)> condition = [limit](uint64_t amt)
    { part1_condition(amt, limit); };
    root.traverse_child_directories(condition);
    cout << "total less than " << limit << " is " << part1_condition(0, 0) << endl;

    // part 2
    uint64_t file_system = 70000000;
    uint64_t necessary = 30000000 - (file_system - root.get_size());
    uint64_t *cur_min = &file_system;
    function<void(uint64_t)> condition2 = [cur_min, necessary](uint64_t amt)
    {
        part2_condition(amt, necessary, cur_min);
    };
    root.traverse_child_directories(condition2);
    cout << "cur min ended up as " << *cur_min << endl;

    // cleanup
    file.close();
    return 0;
}