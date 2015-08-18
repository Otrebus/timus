/* 2046. The First Day at School - http://acm.timus.ru/problem.aspx?num=2046
 *
 * Strategy:
 * N/A :p
 *
 * Performance:
 * Runs in 0.001s, uses 408KB memory
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>

class Entry
{
    std::vector<std::string> v;

public:
    Entry() { }

    Entry(const std::string& input)
    {
        std::istringstream ss(input);
        std::string line;
        std::string word;
        while(ss >> word)
        {
            if(line.length() > 0)
            {
                if(line.length() + 1 + word.length() > 10)
                {
                    v.push_back(line);
                    line = word;
                }
                else
                    line += " " + word;
            }
            else
                line = word;
        }
        if(line.length() > 0)
            v.push_back(line);
    }

    int getHeight() const
    {
        return std::max(1, (int) v.size());
    }

    std::string getRow(int r) const
    {
        return v.size() == 0 || r >= (int) v.size() ? "" : v[r];
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::unordered_map<std::string, int> m = { { "Tuesday", 0 }, { "Thursday", 1 }, 
                                               { "Saturday", 2 } };
    Entry entries[12];
    int n;
    std::cin >> n;
    while(n--)
    {
        std::string line, day;
        int slot;
        std::cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' ); 
        std::getline(std::cin, line);
        std::cin >> day >> slot;
        int pos = m[day] + (slot-1)*3;
        entries[pos] = Entry(line);
    }
    std::string delimiter = "+----------+----------+----------+";
    std::cout << delimiter << std::endl;
    for(int row = 0; row < 4; row++)
    {
        int max = 0;
        for(int column = 0; column < 3; column++)
            max = std::max(max, entries[3*row+column].getHeight());
        for(int line = 0; line < max; line++)
        {
            std::cout << "|";
            for(int column = 0; column < 3; column++)
            {
                std::cout.width(10);
                std::cout << std::left << entries[3*row+column].getRow(line) << "|";
            }
            std::cout << std::endl;
        }
        std::cout << delimiter << std::endl;
    }
}