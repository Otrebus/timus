/* 1866. Poetic Foot - http://acm.timus.ru/problem.aspx?num=1866
 *
 * Strategy:
 * Straightforward. See comments and code.
 *
 * Performance:
 * Linear in the size of the input, runs in 0.001s using 224KB memory (rated 2/288).
 */

#include <sstream>
#include <iostream>
#include <string>

std::pair<std::string, char> readUntil(std::stringstream& ss, std::string chars)
{   // Reads from the current position in the given stream until a delimiter in chars is reached
    std::string ret;
    for(char c; ss >> c; ss)
        if(chars.find(c) != std::string::npos)
            return { ret, c };
        else
            ret.push_back(c);
    return { ret, EOF };
}

std::string getRhythm(std::string line)
{   // Maps a line of poem into a rhythm string of the form "001001001" or similar
    std::string output;
    std::stringstream ss;
    ss << std::noskipws << line;
    bool e = false;
    std::pair<std::string, char> p;
    while(ss)
    {
        p = readUntil(ss, "- []");
        if(p.first.size() > 0)
            output.push_back(e ? '1' : '0');
        e = p.second == '[';
    }
    return output;
}

std::string rhythmString(std::string rhythm)
{   // Maps a rhythm string like "010101" into the corresponding rhythm
    const std::string rhythms[5][2] =
    { { "10", "trochee" },
      { "01", "iamb" },
      { "100", "dactyl" },
      { "010", "amphibrach" },
      { "001", "anapaest" } } ;

    for(int r = 0; r < 5; r++)
    {
        bool fit = true;
        for(int i = 0; i < rhythm.size(); i++)
            fit &= rhythms[r][0][i%rhythms[r][0].size()] == rhythm[i];
        if(fit)
            return rhythms[r][1];
    }  
    return "not a poem";
}

int main()
{
    std::string line;
    std::string type;
    std::getline(std::cin, line);
    while(std::getline(std::cin, line))
    {
        auto lineType = rhythmString(getRhythm(line));
        if(type != "" && lineType != type)
            type = "not a poem";
        else
            type = lineType;
    }
    std::cout << type;
}